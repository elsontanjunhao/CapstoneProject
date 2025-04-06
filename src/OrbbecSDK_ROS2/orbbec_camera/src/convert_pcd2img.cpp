#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>

#include <opencv2/opencv.hpp>
#include <open3d/Open3D.h>

#include <memory>
#include <vector>
#include <cstring>

class PointCloudToImageMapper : public rclcpp::Node {
public:
    PointCloudToImageMapper() : Node("pointcloud_to_image_mapper") {
        // Camera parameters
        translation_ = Eigen::Vector3d(29.8935, -0.109352, 0.57824);
        rotation_ = Eigen::Quaterniond(-0.00138267, -0.00088531, -0.0016808, 0.999997);
        
        // Camera intrinsic matrix
        camera_matrix_ << 
            457.2316589355469, 0.0, 326.6119079589844,
            0.0, 457.2316589355469, 241.35093688964844,
            0.0, 0.0, 1.0;
        
        image_width_ = 640;
        image_height_ = 480;
        
        // Subscriber and publisher
        pointcloud_sub_ = create_subscription<sensor_msgs::msg::PointCloud2>(
            "/camera/depth_registered/points", 
            10, 
            std::bind(&PointCloudToImageMapper::pointcloudCallback, this, std::placeholders::_1)
        );
        
        image_pub_ = create_publisher<sensor_msgs::msg::Image>(
            "/mapped_image", 
            10
        );
    }

private:
    void pointcloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        // Convert ROS PointCloud2 to Open3D point cloud
        open3d::geometry::PointCloud pc_o3d;
        convertRosToOpen3d(*msg, pc_o3d);
        
        // Project point cloud to image
        cv::Mat mapped_image = projectPointCloudToImage(pc_o3d);
        
        // Convert to ROS image message
        auto image_msg = cv_to_ros_image(mapped_image);
        
        // Publish mapped image
        image_pub_->publish(image_msg);
    }
    
    void convertRosToOpen3d(const sensor_msgs::msg::PointCloud2& ros_pc2, 
                             open3d::geometry::PointCloud& pc_o3d) {
        std::vector<Eigen::Vector3d> points;
        
        size_t point_step = ros_pc2.point_step;
        
        for (size_t i = 0; i < ros_pc2.height * ros_pc2.width; ++i) {
            size_t start = i * point_step;
            
            // Safely access point data
            if (start + point_step > ros_pc2.data.size()) break;
            
            float x, y, z;
            std::memcpy(&x, &ros_pc2.data[start], sizeof(float));
            std::memcpy(&y, &ros_pc2.data[start + sizeof(float)], sizeof(float));
            std::memcpy(&z, &ros_pc2.data[start + 2*sizeof(float)], sizeof(float));
            
            points.push_back(Eigen::Vector3d(x, y, z));
        }
        
        pc_o3d.points_ = points;
    }
    
    cv::Mat projectPointCloudToImage(const open3d::geometry::PointCloud& pc_o3d) {
        // Create blank 16-bit depth image
        cv::Mat image = cv::Mat::zeros(image_height_, image_width_, CV_16UC1);
        
        // Project points
        for (const auto& point : pc_o3d.points_) {
            // Project 3D point to 2D using camera matrix
            Eigen::Vector3d projected_point = camera_matrix_ * point;
            
            // Perspective division
            int u = static_cast<int>(projected_point(0) / projected_point(2));
            int v = static_cast<int>(projected_point(1) / projected_point(2));
            
            // Check image bounds
            if (u >= 0 && u < image_width_ && v >= 0 && v < image_height_) {
                // Store depth value in millimeters
                uint16_t depth_mm = static_cast<uint16_t>(point(2) * 1000);
                image.at<uint16_t>(v, u) = depth_mm;
            }
        }
        
        return image;
    }
    
    sensor_msgs::msg::Image cv_to_ros_image(const cv::Mat& cv_image) {
        sensor_msgs::msg::Image image_msg;
        image_msg.height = cv_image.rows;
        image_msg.width = cv_image.cols;
        image_msg.encoding = "mono16";
        image_msg.step = cv_image.cols * 2;  // 2 bytes per pixel
        // Convert image to ROS message data
        image_msg.data.resize(cv_image.total() * cv_image.elemSize());
        std::memcpy(image_msg.data.data(), cv_image.data, image_msg.data.size());
        
        return image_msg;
    }
    
    // Member variables
    Eigen::Vector3d translation_;
    Eigen::Quaterniond rotation_;
    Eigen::Matrix3d camera_matrix_;
    
    int image_width_;
    int image_height_;
    
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_sub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PointCloudToImageMapper>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
