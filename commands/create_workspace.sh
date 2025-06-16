#!/bin/bash

source /opt/ros/${ROS_DISTRO}/setup.bash

cd /app
colcon build --symlink-install --build-base $BUILD_LOCATION --install-base $INSTALL_LOCATION