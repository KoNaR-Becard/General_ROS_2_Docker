#!/bin/bash

source /opt/ros/${ROS_DISTRO}/setup.bash

cd /app

rosdep install --from-paths /app --ignore-src -r -y -q

colcon build --symlink-install --build-base $BUILD_LOCATION --install-base $INSTALL_LOCATION --packages-select $1