#!/bin/bash

source /opt/ros/${ROS_DISTRO}/setup.bash

cd /app

mkdir src

cd src

ros2 pkg create --build-type ament_cmake $1