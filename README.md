# Becard Dron ROS2 container

Container with ROS2 code for Becard dron. It handles development and retail container for deployment on real hardware.

# Development containter

To run dev container execute:

./dev.sh start

Container should start then in seperate terminal you can execute any ros command using:

./dev.sh cmd \<command\>

# Production containter

First build containter execute:

./prod.sh build

Then execute:

./prod.sh start_bg
