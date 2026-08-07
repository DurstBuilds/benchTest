"""Launch all bench-test nodes: ODrive, keyboard RPM, encoder, knee sensor, dead-zone."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """Start odrive, keyboard RPM prompt, encoder, knee sensor, and dead-zone."""
    return LaunchDescription(
        [
            Node(
                package="odrive_interface",
                executable="odrive_interface_node",
                name="odrive_interface_node",
                output="screen",
            ),
            # Reads /dev/tty so "Enter motor RPM:" works in this launch terminal.
            Node(
                package="odrive_interface",
                executable="keyboard_rpm_node",
                name="keyboard_rpm_node",
                output="screen",
                emulate_tty=True,
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
