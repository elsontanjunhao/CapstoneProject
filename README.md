# CapstoneProject

# ROS 2 Workspace for Autonomous Navigation with FastSAM

[![ROS 2 Humble](https://img.shields.io/badge/ROS-2_Humble-brightgreen)](https://docs.ros.org/en/humble/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

![System Architecture](docs/system_architecture.png)

## 📂 Repository Structure
turtlebot3_ws/
├── src/
│ ├── OrbbecSDK_ROS2/ # Orbbec camera integration
│ │ ├── src/
│ │ │ ├── convert_pcd2img.cpp # Point cloud to image converter
│ │ │ └── depthviewer.cpp # Depth data visualizer
│ ├── ultralytics_ros/ # SAM integration
│ │ ├── launch/
│ │ │ └── detect_obstacle.launch.py
│ │ ├── scripts/
│ │ │ └── sam_node.py
│ │ └── msg/
│ │ └── Segmentation.msg
│ └── crewAI/ # AI agent framework
└── build/

## 🚀 Features
- Real-time obstacle detection with SAM (Segment Anything Model)
- RGBD camera integration with depth processing
- Custom point cloud to 2D image conversion
- Autonomous navigation stack integration

## ⚙️ Prerequisites
- **Hardware**: NVIDIA Jetson Orin Nano
- **OS**: Ubuntu 22.04
- **ROS**: Humble Hawksbill
- **Dependencies**:
  ```bash
  sudo apt install python3-pip cmake libopencv-dev
  pip install torch torchvision ultralytics
