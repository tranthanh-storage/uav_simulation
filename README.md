# UAV Simulation

## Project Overview
This project delivers a C++ based UAV simulation framework using ROS 2 and PX4, featuring two core modes of operation: interactive manual control and fully autonomous offboard control. The project is intended to provide a foundational solution and platform that enables researchers to quickly and professionally engage with and implement complex UAV algorithms.

## Project Setup

### Step 1: Basic setup

- Install [ROS 2 Humble](https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html) on Ubuntu 22.04 (recommended)

- Install [ROS 2 Jazzy](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html) on Ubuntu 24.04

- Install colcon to build ROS 2 packages

### Step 2: Download firmware

- Download [PX4 firmware](https://docs.px4.io/main/en/dev_setup/building_px4.html) (v1.15.1)
```
git clone --branch v1.15.1 --recursive https://github.com/PX4/PX4-Autopilot.git
```

- Install [Gazebo-Classic 11](https://docs.px4.io/main/en/sim_gazebo_classic/)
```
sudo apt remove gz-harmonic
sudo apt install aptitude
sudo aptitude install gazebo libgazebo11 libgazebo-dev
sudo apt install libopencv-dev protobuf-compiler libeigen3-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```
- In case Ubuntu 24.04 and ROS2 Jazzy, install [Gazebo-Harmonic] (https://gazebosim.org/docs/harmonic/install_ubuntu/)

- Install [XRCE-DDS](https://docs.px4.io/main/en/ros2/user_guide.html#setup-micro-xrce-dds-agent-client) (bridge between PX4 and ROS 2)
```
git clone -b master https://github.com/eProsima/Micro-XRCE-DDS-Agent.git
cd Micro-XRCE-DDS-Agent
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig /usr/local/lib/
```

- Clone [uav_simulation]() project

### Step 3: Build firmware

- Build PX4 firmware (Using a Simulator SITL)
```
cd PX4-Autopilot
make px4_sitl_default gazebo-classic_iris
```
in case using Gazebo Harmonic/Ionic/Jetty
```
cd PX4-Autopilot
make px4_sitl gz_x500
```

- Build uav_simualtion project
```
cd uav_simualtion
colcon build
source install/setup.bash
```

### Step 4: Run project

Open some terminals

- Terminal 1: run PX4 firmware
```
cd PX4-Autopilot
make px4_sitl_default gazebo-classic_iris
```

- Terminal 2: run the comunication of ROS 2 and PX4
```
cd MicroXRCEAgent
cd build
./MicroXRCEAgent udp4 -p 8888
```

- Terminal 3: run uav_simulation project for arm and take off
```
cd uav_simualtion
colcon build
source install/setup.bash
ros2 run drone_control takeoff
```
or for a short flight
```
cd uav_simualtion
colcon build
source install/setup.bash
ros2 run drone_control flight
```
