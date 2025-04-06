#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import rclpy
import os
import subprocess
from rclpy.node import Node
from rclpy.qos import qos_profile_system_default
from rcl_interfaces.msg import Log
from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node as LaunchNode
from launch import LaunchService

class RosoutListener(Node):
    def __init__(self):
        super().__init__('rosout_listener')
        self.subscription = self.create_subscription(
            Log,
            '/rosout',
            self.listener_callback,
            qos_profile_system_default)

    def listener_callback(self, msg):
        if 'Goal failed' in msg.msg:
            self.get_logger().info('Detected "Goal failed" message. Launching detect_obstacle.launch.py...')
            self.launch_detect_obstacle()

    def launch_detect_obstacle(self):
        """Launch detect_obstacle.launch.py when a goal fails."""
        launch_file_path = os.path.expanduser("~/turtlebot3_ws/src/ultralytics_ros/launch/detect_obstacle.launch.py")

        if os.path.exists(launch_file_path):
            try:
                subprocess.Popen(["ros2", "launch", "ultralytics_ros", "detect_obstacle.launch.py"])
                self.get_logger().info("✅ Successfully launched detect_obstacle.launch.py")
            except Exception as e:
                self.get_logger().error(f"❌ Failed to launch detect_obstacle.launch.py: {e}")
        else:
            self.get_logger().error(f"❌ Launch file not found: {launch_file_path}")

def main(args=None):
    rclpy.init(args=args)
    node = RosoutListener()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()