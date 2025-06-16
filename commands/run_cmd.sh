#!/bin/bash

source /opt/ros/${ROS_DISTRO}/setup.bash
source /app/$INSTALL_LOCATION/setup.bash

exec "$@"