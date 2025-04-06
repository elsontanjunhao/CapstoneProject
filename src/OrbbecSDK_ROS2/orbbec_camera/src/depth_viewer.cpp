#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <limits>
#include <cmath>

namespace orbbec_camera {

class DepthViewer : public rclcpp::Node {
public:
    explicit DepthViewer(const rclcpp::NodeOptions &options) 
        : Node("depth_viewer", options) {
        // Declare parameters for filtering
        declare_parameter("filter_x_min", std::numeric_limits<float>::lowest());
        declare_parameter("filter_x_max", std::numeric_limits<float>::max());
        declare_parameter("filter_y_min", std::numeric_limits<float>::lowest());
        declare_parameter("filter_y_max", std::numeric_limits<float>::max());
        declare_parameter("filter_z_min", std::numeric_limits<float>::lowest());
        declare_parameter("filter_z_max", std::numeric_limits<float>::max());

        // Subscribe to depth image topic
        depth_sub_ = create_subscription<sensor_msgs::msg::Image>(
            "/mapped_image", 10,
            std::bind(&DepthViewer::processDepthImage, this, std::placeholders::_1));

        // Publish closest depth point
        closest_point_pub_ = create_publisher<geometry_msgs::msg::Point>(
            "/camera/closest_depth_point", 10);

        RCLCPP_INFO(get_logger(), "DepthViewer node initialized with filtering capabilities.");
    }

private:
    void processDepthImage(const sensor_msgs::msg::Image::SharedPtr msg) {
        try {
            // Retrieve current filter parameters
            float x_min = get_parameter("filter_x_min").as_double();
            float x_max = get_parameter("filter_x_max").as_double();
            float y_min = get_parameter("filter_y_min").as_double();
            float y_max = get_parameter("filter_y_max").as_double();
            float z_min = get_parameter("filter_z_min").as_double();
            float z_max = get_parameter("filter_z_max").as_double();

            // Convert ROS image message to OpenCV format
            cv::Mat depth = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1)->image;

            // Find closest depth point with filtering and depth discontinuity exclusion
            float minDistance = std::numeric_limits<float>::max();
            int minX = -1, minY = -1;
            const float depth_gradient_threshold = 50.0; // Threshold for depth gradient

            for (int y = 1; y < depth.rows - 1; ++y) {
                for (int x = 1; x < depth.cols - 1; ++x) {
                    float distance = depth.at<uint16_t>(y, x);

                    // Calculate gradients
                    float dx = std::abs(static_cast<float>(depth.at<uint16_t>(y, x + 1)) -
                                        static_cast<float>(depth.at<uint16_t>(y, x - 1)));
                    float dy = std::abs(static_cast<float>(depth.at<uint16_t>(y + 1, x)) -
                                        static_cast<float>(depth.at<uint16_t>(y - 1, x)));

                    // Exclude points near sharp depth changes
                    if (dx > depth_gradient_threshold || dy > depth_gradient_threshold) {
                        continue;
                    }

                    // Apply filtering conditions
                    bool passes_x_filter = (x >= x_min && x <= x_max);
                    bool passes_y_filter = (y >= y_min && y <= y_max);
                    bool passes_z_filter = (distance >= z_min && distance <= z_max);

                    if (distance > 0 && 
                        passes_x_filter && 
                        passes_y_filter && 
                        passes_z_filter && 
                        distance < minDistance) {
                        minDistance = distance;
                        minX = x;
                        minY = y;
                    }
                }
            }

            // Publish closest point
            if (minX != -1 && minY != -1) {
                geometry_msgs::msg::Point point_msg;
                point_msg.x = minX;
                point_msg.y = minY;
                point_msg.z = minDistance;
                closest_point_pub_->publish(point_msg);
            } else {
                RCLCPP_WARN(get_logger(), "No depth point found after filtering.");
            }
        } catch (const std::exception &e) {
            RCLCPP_ERROR(get_logger(), "Error processing depth image: %s", e.what());
        }
    }

    // Subscriber for depth image
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_sub_;

    // Publisher for the closest depth point
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr closest_point_pub_;
};

}  // namespace orbbec_camera

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(orbbec_camera::DepthViewer)

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    auto node = std::make_shared<orbbec_camera::DepthViewer>(options);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

