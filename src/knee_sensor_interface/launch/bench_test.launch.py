"""Launch all bench-test nodes: ODrive, encoder, knee sensor, dead-zone."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """Start odrive, encoder, knee sensor, and calculate_dead_zone together."""
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
                package="knee_sensor_interface",
                executable="calculate_dead_zone",
                name="calculate_dead_zone",
                output="screen",
            ),
        ]
    )
