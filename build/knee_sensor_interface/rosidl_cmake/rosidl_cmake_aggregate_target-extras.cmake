# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target knee_sensor_interface::knee_sensor_interface
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${knee_sensor_interface_TARGETS}.
if(knee_sensor_interface_TARGETS AND NOT TARGET knee_sensor_interface::knee_sensor_interface)
  add_library(knee_sensor_interface::knee_sensor_interface INTERFACE IMPORTED)
  set_target_properties(knee_sensor_interface::knee_sensor_interface PROPERTIES
    INTERFACE_LINK_LIBRARIES "${knee_sensor_interface_TARGETS}")
endif()
