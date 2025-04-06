import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2, Image, CameraInfo
import numpy as np
import open3d as o3d
import cv2
import struct
import sys

class PointCloudToImageMapper(Node):
    def __init__(self):
        super().__init__('pointcloud_to_image_mapper')
        
        # Camera parameters based on the provided transformation info
        self.translation = np.array([29.8935, -0.109352, 0.57824])
        self.rotation = np.array([-0.00138267, -0.00088531, -0.0016808, 0.999997])
        
        # Camera intrinsic parameters from camera_info
        self.camera_matrix = np.array([
            [457.2316589355469, 0.0, 326.6119079589844],
            [0.0, 457.2316589355469, 241.35093688964844],
            [0.0, 0.0, 1.0]
        ])
        
        self.image_width = 640
        self.image_height = 480
        
        # Subscribers and publishers
        self.pointcloud_sub = self.create_subscription(
            PointCloud2, 
            '/camera/depth_registered/points', 
            self.pointcloud_callback, 
            10
        )
        
        self.image_pub = self.create_publisher(
            Image, 
            '/mapped_image', 
            10
        )
        
    def ros_to_open3d(self, ros_pc2):
        """
        Convert ROS PointCloud2 to Open3D point cloud
        
        Args:
            ros_pc2 (PointCloud2): ROS point cloud message
        
        Returns:
            open3d.geometry.PointCloud: Converted point cloud
        """
        # Create Open3D point cloud
        pc_o3d = o3d.geometry.PointCloud()
        
        # Parse point cloud data
        point_step = ros_pc2.point_step
        row_step = ros_pc2.row_step
        
        # Extract point coordinates (assuming float32 x, y, z)
        points = []
        for i in range(ros_pc2.height * ros_pc2.width):
            start = i * point_step
            end = start + point_step
            point_data = ros_pc2.data[start:end]
            
            # Unpack x, y, z coordinates (assumes little-endian float32)
            try:
                x, y, z = struct.unpack_from('<fff', point_data)
                points.append([x, y, z])
            except struct.error:
                # Skip unparseable points
                continue
        
        pc_o3d.points = o3d.utility.Vector3dVector(points)
        return pc_o3d
    
    def project_pointcloud_to_image(self, pc_o3d):
        """
        Project point cloud to 2D image plane with depth handling
        
        Args:
            pc_o3d (open3d.geometry.PointCloud): Input point cloud
        
        Returns:
            numpy.ndarray: Projected 2D image with depth information
        """
        # Create blank image with depth information (16-bit unsigned)
        image = np.zeros((self.image_height, self.image_width), dtype=np.uint16)
        
        # Convert point cloud to numpy array
        points = np.asarray(pc_o3d.points)
        
        # Project 3D points to 2D image plane
        for point in points:
            # Project 3D point to 2D using camera matrix
            # Assumes points are in camera coordinate system
            projected_point = self.camera_matrix @ point
            
            # Perspective division
            u = int(projected_point[0] / projected_point[2])
            v = int(projected_point[1] / projected_point[2])
            
            # Check if point is within image bounds
            if 0 <= u < self.image_width and 0 <= v < self.image_height:
                # Store depth value, converting to millimeters (if not already)
                depth_mm = int(point[2] * 1000)  # Convert to millimeters
                image[v, u] = depth_mm
        
        return image
    
    def depth_to_color(self, depth):
        """
        Convert depth to color (blue to red gradient)
        
        Args:
            depth (float): Depth value
        
        Returns:
            tuple: BGR color
        """
        # Blue (far) to Red (near) gradient
        b = int(255 * (1 - depth / 10))
        r = int(255 * (depth / 10))
        return (b, 0, r)
    
    def pointcloud_callback(self, msg):
        """
        Callback for incoming point cloud messages
        
        Args:
            msg (PointCloud2): Incoming point cloud message
        """
        # Convert ROS PointCloud2 to Open3D point cloud
        pc_o3d = self.ros_to_open3d(msg)
        
        # Project point cloud to image
        mapped_image = self.project_pointcloud_to_image(pc_o3d)

        # Convert numpy image to ROS Image message
        image_msg = self.cv_to_ros_image(mapped_image)
        print(image_msg.height)
        # Publish mapped image
        self.image_pub.publish(image_msg)
    
    def cv_to_ros_image(self, cv_image):
        """
        Convert OpenCV image to ROS Image message
        
        Args:
            cv_image (numpy.ndarray): OpenCV image
        
        Returns:
            Image: ROS Image message
        """
        image_msg = Image()
        image_msg.height = cv_image.shape[0]
        image_msg.width = cv_image.shape[1]
        image_msg.encoding = 'mono16'  # 16-bit depth encoding
        image_msg.step = cv_image.shape[1] * 2  # 2 bytes per pixel
        image_msg.data = cv_image.tobytes()
        
        return image_msg

def main(args=None):
    rclpy.init(args=args)
    mapper = PointCloudToImageMapper()
    rclpy.spin(mapper)
    mapper.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
