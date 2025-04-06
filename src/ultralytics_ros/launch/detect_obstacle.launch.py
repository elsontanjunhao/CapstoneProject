#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    # Declare launch arguments
    args = [
        DeclareLaunchArgument('sam_model', default_value='FastSAM-s.pt'),
        DeclareLaunchArgument('input_image', default_value='camera/color/image_raw'),
        DeclareLaunchArgument('input_point_prompt', default_value='camera/closest_depth_point'),
        DeclareLaunchArgument('result_image_topic', default_value='sam_result_image'),
        DeclareLaunchArgument('device', default_value='cuda:0'),
        DeclareLaunchArgument('camera_name', default_value='camera'),
        DeclareLaunchArgument('enable_test_mode', default_value='false')
    ]

    # Include Orbbec camera launch file
    orbbec_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare('orbbec_camera'), '/launch/dabai.launch.py']
        ),
        launch_arguments={
            'camera_name': LaunchConfiguration('camera_name')
        }.items()
    )
    
    # Point Cloud to Image Mapper Node
    pointcloud_to_image_node = Node(
        package='orbbec_camera',  # Use the same package as your other nodes
        executable='pointcloud_to_image_mapper_node',  # The executable name from CMakeLists.txt
        name='pointcloud_to_image_mapper',
        output='screen'
    )

    # SAM Segmentation Node
    sam_node = Node(
        package='ultralytics_ros',
        executable='sam_node.py',
        name='sam_segmentation_node',
        parameters=[{
            'sam_model': LaunchConfiguration('sam_model'),
            'input_image': LaunchConfiguration('input_image'),
            'input_point_prompt': LaunchConfiguration('input_point_prompt'),
            'result_image_topic': LaunchConfiguration('result_image_topic'),
            'device': LaunchConfiguration('device'),
            'enable_test_mode': LaunchConfiguration('enable_test_mode')
        }],
        output='screen'
    )

    # VLM Node
    vlm_node = Node(
        package='ultralytics_ros',  # Replace with your actual package name
        executable='VLM.py',  # The name of the executable script
        name='vlm_node',
        output='screen'
    )

    # Create launch description
    ld = LaunchDescription(
        args + [
            orbbec_launch,
            pointcloud_to_image_node,
            sam_node,
            vlm_node,
        ]
    )

    return ld