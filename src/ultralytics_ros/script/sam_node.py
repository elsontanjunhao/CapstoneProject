#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import cv_bridge
import numpy as np
import rclpy
import sys
import cv2
import os
import time
from datetime import datetime
from ament_index_python.packages import get_package_share_directory
from rclpy.node import Node
from sensor_msgs.msg import Image
from geometry_msgs.msg import Point
from std_msgs.msg import Int32MultiArray, Bool
from ultralytics import SAM, FastSAM
from vision_msgs.msg import Detection2DArray, Detection2D, ObjectHypothesisWithPose
from ultralytics_ros.msg import Segmentation

class SAMSegmentationNode(Node):
    def __init__(self):
        super().__init__("sam_segmentation_node")
        
        # Image setting
        self.rgb_frame = (640,400)
        self.depth_frame = (640,400)
        # Blurriness threshold to accept image
        self.BLURRINESS_THRESHOLD = 150.0  # Lower values to reject blur image, Higher value to accept blurrier
        # Bounding Box threshold to avoid capturing background (Number above not included)
        self.BBOX_THRESHOLD = 0.8
        
        # # Point Stability Configuration
        # self.POINT_CHANGE_THRESHOLD = 20  # Pixel distance threshold for point stability
        # self.stable_query_point = None
        # self.last_stable_point = None
        # self.last_stable_point_timestamp = None
        # self.LAST_POINT_STABILITY_TIMEOUT = 0.5  # 500 milliseconds
        
        # Confidence level to accept detection 
        self.seg_conf = 0.7
        
        self.new_point = np.array([[0, 0]])
        self.aligned_point = 0
        self.point_message_count = 0

        self.ack_received = False
        self.ros_image = None
        self.finalcheck = None
        
        # Existing parameter declarations and model loading
        self.declare_parameter("sam_model", "mobile_sam.pt") #(Faster but less accurate: FastSAM-s.pt)
        self.declare_parameter("input_image", "camera/color/image_raw")
        self.declare_parameter("input_point_prompt", "camera/closest_depth_point")
        self.declare_parameter("result_image_topic", "sam_result_image")
        self.declare_parameter("device", "cuda:0")
        # self.declare_parameter("result_topic", "sam_segmentation")
        self.declare_parameter("enable_test_mode", False)
        
        # Load SAM model
        path = get_package_share_directory("ultralytics_ros")
        sam_model = self.get_parameter("sam_model").get_parameter_value().string_value
        if 'sam2' in sam_model or 'mobile' in sam_model: 
            self.model = SAM(f"{path}/models/{sam_model}")
        elif 'FastSAM' in sam_model:
            self.model = FastSAM(f"{path}/models/{sam_model}")
        else:
            print("No model found in ultralytics")
            sys.exit()
        
        # Initialize bridge and other components
        self.bridge = cv_bridge.CvBridge()
        
        input_topic = self.get_parameter("input_image").get_parameter_value().string_value
        point_topic = self.get_parameter("input_point_prompt").get_parameter_value().string_value
        result_image_topic = self.get_parameter("result_image_topic").get_parameter_value().string_value
        # result_imgseg_topic = self.get_parameter("result_topic").get_parameter_value().string_value
        self.test_mode = self.get_parameter("enable_test_mode").get_parameter_value().bool_value
        
        # Create subscription for image and query points
        self.create_subscription(Image, input_topic, self.image_callback, 10)
        self.create_subscription(Point, point_topic, self.add_query_points, 10)
        self.create_subscription(Bool, "det_obs_ack", self.ack_callback, 10)

        
        # Create publishers
        # self.results_pub = self.create_publisher(Segmentation, result_topic, 10)
        self.result_image_pub = self.create_publisher(Image, result_image_topic, 10)
        self.det_obs_img_pub = self.create_publisher(Image, 'det_obs_img', 10)
        self.timer = self.create_timer(1, self.det_obs_img_callback)
        self.bridge = cv_bridge.CvBridge()
        
        # Query point label (consistent across calls) [1]-foreground, [0]-background, Depth frame = (640 x 400)
        self.query_labels = [1]
        
        # Initialize variables for saving images
        if not self.test_mode:
            self.collected_images = []
            self.last_save_time = 0
            self.MAX_IMAGES = 5
            self.save_directory = os.path.expanduser("~/turtlebot3_ws/src/ultralytics_ros/runs/detection")
            os.makedirs(self.save_directory, exist_ok=True)

    def det_obs_img_callback(self):
        if self.finalcheck:
            self.det_obs_img_pub.publish(self.ros_image)
            self.get_logger().info("Concluded final image, publishing to VLM")
        else:
            self.get_logger().warn("No final detected image")
            return

    def ack_callback(self, msg):
        """Handle acknowledgment from the subscriber."""
        
        if msg.data:
            self.get_logger().info("Acknowledgement Received.")
            self.ack_received = True
        else:
            self.get_logger().info("No Acknowledgement Received.")
            self.ack_received = False

    def align_point_coordinates(self, msg, rgb_size, depth_size):
        """
        Align depth frame coordinates to RGB frame
        
        Key Principles:
        1. Preserve horizontal (x) coordinate
        2. Vertically center the depth frame within RGB frame
        3. Ensure coordinates remain within RGB frame bounds
        """
        
        # Vertical Centering Calculation
        # Distribute remaining vertical pixels evenly
        vertical_offset = (rgb_size[1] - depth_size[1]) // 2
        
        # Map coordinates with centering
        aligned_x = msg.x  # Horizontal coordinate remains unchanged
        aligned_y = msg.y + vertical_offset
        
        # Safety check: Ensure point is within RGB frame
        aligned_x = max(0, min(aligned_x, rgb_size[0] - 1))
        aligned_y = max(0, min(aligned_y, rgb_size[1] - 1))
        
        return (aligned_x, aligned_y)
    
    def add_query_points(self, msg):
        """
        Process and potentially update query points with stability check.
        
        Args:
            msg (Point): Incoming point message
        """
        if not self.finalcheck:
            # Convert point to NumPy array
            print(msg)
            self.point_message_count += 1
            self.get_logger().info(
                f"Message #{self.point_message_count}: "
                f"Received Point(x={msg.x}, y={msg.y}, z={msg.z}) "
            )
            
            # Align to RGB frame
            # self.aligned_point = self.align_point_coordinates(msg, self.rgb_frame, self.depth_frame)
            

            self.new_point = np.array([[msg.x, msg.y]])
            # self.new_point = np.array([self.aligned_point])
        else:
            return


    def image_callback(self, msg):
        """
        Process image with stable query point.
        
        Args:
            msg (Image): Incoming image message
        """
        if not self.finalcheck:
        
            # Convert ROS image to OpenCV image
            cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
            
            start_time = time.time()  
            
            # Perform segmentation with stable query point
            device = self.get_parameter("device").get_parameter_value().string_value or None
            
            results = self.model.predict(
                source=cv_image,
                conf=self.seg_conf,
                half=True,
                points=self.new_point,
                labels=self.query_labels,
                device=device,
                batch=1,
            #save=True,
            project=os.path.expanduser("~/turtlebot3_ws/src/ultralytics_ros/runs/") ,
            #save_crop=True,
            )


            
            if results is not None:
                #Publish segmentation results
                sam_result_image_msg = self.create_result_image(results)
                
                if sam_result_image_msg is not None:
                    self.result_image_pub.publish(sam_result_image_msg)
                    inference_time = time.time() - start_time
                    self.get_logger().info(f"FastSAM Inference Time: {inference_time:.4f} seconds")
                else:
                    self.get_logger().info("No valid result image to publish.")
            else:
                self.get_logger().info("results have completed")
                return
        else:
            # self.get_logger().info("Final image had collected")
            if self.ack_received:
                self.get_logger().info("object detection system shutting down!")
                raise SystemExit




    def save_image(self, image, score):
        """Collect images, save all at once, and save the highest-scored image separately."""

        current_time = time.time()

        # Check if enough time has passed since the last save
        if current_time - self.last_save_time >= 1:
            # Add the image and its score to the collection
            self.collected_images.append((image, score))
            
            # Update the last save time
            self.last_save_time = current_time
            
            # Log the current collection status
            self.get_logger().info(f"Collected image {len(self.collected_images)}/{self.MAX_IMAGES}")
            
            # Check if we've collected the maximum number of images
            if len(self.collected_images) >= self.MAX_IMAGES:
                # Generate directory name based on current date and time
                folder_name = datetime.now().strftime("%d%m%y_%H_%M")
                save_path = os.path.join(self.save_directory, folder_name)
                os.makedirs(save_path, exist_ok=True)  # Ensure the directory exists

                # Save all collected images
                top_confidence = -1
                top_conf_image = None

                for idx, (collected_image, confidence) in enumerate(self.collected_images, 1):
                    # Save each image
                    image_name = f"detection_{idx}.jpg"
                    image_path = os.path.join(save_path, image_name)
                    cv2.imwrite(image_path, collected_image)
                    self.get_logger().info(f"Saved image: {image_path}")

                    # Check if this image has the highest confidence
                    if confidence > top_confidence:
                        top_confidence = confidence
                        top_conf_image = collected_image

                    if idx == self.MAX_IMAGES: 
                        self.finalcheck = True

                # Save the top-confidence image
                if top_conf_image is not None:
                    top_conf_path = os.path.join(save_path, "top_conf_img.jpg")
                    cv2.imwrite(top_conf_path, top_conf_image)
                    self.get_logger().info(f"Saved 5 images and top confidence image: {top_conf_path} ")

                    # Publish the top confidence image to "det_obs_img" topic
                    self.ros_image = self.bridge.cv2_to_imgmsg(top_conf_image, encoding="bgr8")
                    # self.chk_acknowledgement()
                else:
                    self.get_logger().error("Cannot find top confidence image")

    # def chk_acknowledgement(self):
    #     if not self.ack_received: 
    #         self.get_logger().warn("No acknowledgment received. Waiting for acknowledgement")
    #         rclpy.spin_once(self,timeout_sec=0.1)
    #         self.chk_acknowledgement()
    #     else:
    #         self.get_logger().info("Acknowledgment received. Exiting node.")
    #         # Exit the node after saving\
    #         rclpy.shutdown()
    #         sys.exit()

    def estimate_image_sharpness(self, image):
        """
        Estimate image sharpness using Variance of Laplacian method
        Lower values indicate blurrier images
        """
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        laplacian_var = cv2.Laplacian(gray, cv2.CV_64F).var()
        return laplacian_var

    def create_segmentation_masks(self, results):
        masks_msg = []
        for result in results:
            if hasattr(result, "masks") and result.masks is not None:
                for mask_tensor in result.masks:
                    mask_numpy = (
                        np.squeeze(mask_tensor.data.to("cpu").detach().numpy()).astype(
                            np.uint8
                        )
                        * 255
                    )
                    mask_image_msg = self.bridge.cv2_to_imgmsg(
                        mask_numpy, encoding="mono8"
                    )
                    masks_msg.append(mask_image_msg)
        return masks_msg

    def create_result_image(self, results):
        """
        Plot segmentation/detection results on the original image with conditions:
        1) Image < Blurriness threshold
        2) detection results < specific maximum boundingbox area size
        3) If multiple bounding boxes in each result, display only the one with the highest confidence.
        """
        if not results:
            self.get_logger().warn("No results returned.")
            return None

        # Frame dimensions
        frame_height, frame_width = results[0].orig_shape[:2]
        frame_area = frame_height * frame_width

        # Copy the original image to avoid modifying the entire frame
        result_image = results[0].orig_img.copy()

        sharpness = self.estimate_image_sharpness(result_image)
        
        # Removes blur images
        if sharpness < self.BLURRINESS_THRESHOLD: 
            # self.get_logger().info(f"Image too blurry. Sharpness: {sharpness:.2f}")
            return None

        # Filter results
        filtered_results = []
        

        for result in results:
            if result.boxes is not None and result.masks is not None:
                for box, score, mask in zip(result.boxes.xyxy, result.boxes.conf, result.masks.data):
                    # Bounding box dimensions
                    x_min, y_min, x_max, y_max = map(int, box)
                    bbox_width = x_max - x_min
                    bbox_height = y_max - y_min
                    bbox_area = bbox_width * bbox_height

                    # Condition 1: Skip bounding boxes > specific size of the frame area
                    if bbox_area > self.BBOX_THRESHOLD * frame_area:
                        # self.get_logger().info(f"Skipping bbox with area {bbox_area}, >90% of frame area.")
                        continue

                    # Add result for sorting by confidence
                    filtered_results.append((score, box, mask))

        # Sort by confidence and select the highest confidence result
        if filtered_results:
            filtered_results.sort(key=lambda x: x[0], reverse=True)  # Sort by score (confidence)
            _, (x_min, y_min, x_max, y_max), best_mask = filtered_results[0]

            # # Overlay the mask on the result image (Visualise Segmentation)
            # mask_resized = cv2.resize(best_mask.cpu().numpy(), (frame_width, frame_height), interpolation=cv2.INTER_NEAREST)
            # mask_colored = np.zeros_like(result_image)
            # mask_colored[:, :, 0] = (mask_resized * 255).astype(np.uint8)  # Blue mask overlay
            # # Blend the mask with the original image
            # result_image = cv2.addWeighted(result_image, 0.8, mask_colored, 0.4, 0)

            # Ensure bounding box coordinates are integers
            x_min, y_min, x_max, y_max = map(int, (x_min, y_min, x_max, y_max))

            # Draw bounding box on the image
            cv2.rectangle(result_image, (x_min, y_min), (x_max, y_max), (0, 0, 255), 2)  # blue box
            cv2.putText(
                result_image,
                f"Conf: {filtered_results[0][0]:.2f}",
                (x_min, y_min - 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                fontScale=0.6,
                color=(0, 0, 255),
                thickness=2,
            )
            if self.test_mode:
                # Safe point drawing
                try:
                    cv2.circle(
                        result_image, 
                        (int(self.new_point[0][0]), int(self.new_point[0][1])), 
                        3, 
                        (0,0,255), 
                        1
                    )

                    
                except Exception as e:
                    self.get_logger().error(f"Error drawing point: {e}")
        
            else:
                self.save_image(result_image, float(filtered_results[0][0]))

        # Convert the final result to a ROS Image message
        result_image_msg = self.bridge.cv2_to_imgmsg(result_image, encoding="bgr8")

        return result_image_msg


def main(args=None):
    try:
        rclpy.init(args=args)
        node = SAMSegmentationNode()
        try:
            rclpy.spin(node)
        except SystemExit:
            rclpy.logging.get_logger("sam_segmentation_node").info('System shutdown')
        finally:
            node.destroy_node()
            rclpy.shutdown()
            sys.exit(0)
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)
if __name__ == "__main__":
    main()
