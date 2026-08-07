"""Launch hardware nodes, experiment manager, and rosbag for latch-angle sweep."""

from datetime import datetime
from pathlib import Path

from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node


def generate_launch_description():
    """Start ODrive, encoder, knee Hall, experiment manager, and bag record."""
    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    bag_dir = str(Path.cwd() / f"latch_angle_experiment_{stamp}")

    return LaunchDescription(
        [
            Node(
                package="odrive_interface",
                executable="odrive_interface_node",
                name="odrive_interface_node",
                output="screen",
            ),
            Node(
                package="encoder_interface",
                executable="encoder_interface_node",
                name="encoder_interface_node",
                output="screen",
            ),
            Node(
                package="knee_sensor_interface",
                executable="knee_sensor_interface_node",
                name="knee_sensor_interface_node",
                output="screen",
            ),
            Node(
                package="experiment_manager",
                executable="experiment_manager_node",
                name="experiment_manager_node",
                output="screen",
            ),
            ExecuteProcess(
                cmd=[
                    "ros2",
                    "bag",
                    "record",
                    "-o",
                    bag_dir,
                    "motor_rpm",
                    "arm1_mean_angle",
                    "arm2_mean_angle",
                ],
                output="screen",
            ),
        ]
    )
