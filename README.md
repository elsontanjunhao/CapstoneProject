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
```  
## 🛠️ Installation Guide
1. Workspace Setup
```bash
mkdir -p ~/turtlebot3_ws/src
cd ~/turtlebot3_ws/src
git clone https://github.com/yourusername/turtlebot3_ws.git
```
## 2. Package Installation
OrbbecSDK_ROS2
```bash
cd ~/turtlebot3_ws/src
git clone https://github.com/orbbec/OrbbecSDK_ROS2.git
# Build Open3D with GUI disabled
cmake .. -DBUILD_GUI=OFF
```
ultralytics_ros
```bash
git clone https://github.com/Alpaca-zip/ultralytics_ros.git
# Install PyTorch for Jetson
sudo apt-get install python3-pip libopenblas-base libopenmpi-dev
pip3 install numpy torch-2.3.0a0+8d0a929.nv24.04-cp38-cp38-linux_aarch64.whl
```
3. Build Workspace
```bash
cd ~/turtlebot3_ws
colcon build --symlink-install
source install/setup.bash
```

🖥️ Usage
```bash
# Launch obstacle detection system
ros2 launch ultralytics_ros detect_obstacle.launch.py

# Enable test mode
ros2 launch ultralytics_ros detect_obstacle.launch.py enable_test_mode:=True
```
