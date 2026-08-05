cmake_minimum_required(VERSION 3.8)
project(knee_sensor_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

find_library(GPIOD_LIBRARY gpiod REQUIRED)
find_path(GPIOD_INCLUDE_DIR gpiod.h REQUIRED)

# Add more interface files here as needed, e.g.:
#   "srv/Calibrate.srv"
#   "action/CollectData.action"
set(msg_files
  "msg/KneeSensor.msg"
)
set(srv_files
)
set(action_files
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  ${action_files}
  DEPENDENCIES std_msgs builtin_interfaces
)

rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

add_executable(knee_sensor_interface_node src/knee_sensor_interface_node.cpp)
ament_target_dependencies(knee_sensor_interface_node rclcpp)
target_include_directories(knee_sensor_interface_node PRIVATE ${GPIOD_INCLUDE_DIR})
target_link_libraries(knee_sensor_interface_node "${cpp_typesupport_target}" ${GPIOD_LIBRARY})

add_executable(calculate_dead_zone src/calculate_dead_zone.cpp)
ament_target_dependencies(calculate_dead_zone rclcpp std_msgs)
target_link_libraries(calculate_dead_zone "${cpp_typesupport_target}")

install(TARGETS knee_sensor_interface_node calculate_dead_zone
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_dependencies(rosidl_default_runtime)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
