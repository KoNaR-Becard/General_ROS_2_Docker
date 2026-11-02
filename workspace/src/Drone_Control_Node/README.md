# Dron control node

A simple node used to control dron linear and angular velocities using `geometry_msgs/TwistStamped` message.
After startup it arms drone and fly it to its initial height. When timeout occurs or drone reach its starting
height, it starts for lising for twist messages.

# Topics
- /cmd_vel_stamped - geometry_msgs/TwistStamped - twist stamped input message.
- /fmu/out/vehicle_local_position_v1 - px4_msgs/VehicleLocalPosition - drone current position.
- /fmu/in/trajectory_setpoint - px4_msgs/TrajectorySetpoint - topic used to set drone velocities.
- /fmu/in/offboard_control_mode - px4_msgs/OffboardControlMode - topic used for setting drone control mode.
- /fmu/in/vehicle_command - px4_msgs/VehicleCommand - topic used for general drone configuration.

# Parameters
- starting_height - drone initial height, 2.0 meters by default.
- start_timeout - start sequence timeout, 30 seconds by default.