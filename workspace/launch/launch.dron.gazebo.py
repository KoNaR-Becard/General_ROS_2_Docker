import os
import logging
from ament_index_python.packages import get_package_share_directory,get_package_prefix
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node

from launch.actions import SetEnvironmentVariable

def generate_launch_description():
    
    # Specify the name of the package and path to xacro file within the package
    
    bridge_params = os.path.join(
    '/app',
    'cfg',
    'gazebo_bridge.yaml'
    )
    
    gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '--ros-args',
            '-p',
            f'config_file:={bridge_params}',
        ],
        output='screen',
    )

    # Run the node
    return LaunchDescription([
        gz_bridge
    ])

