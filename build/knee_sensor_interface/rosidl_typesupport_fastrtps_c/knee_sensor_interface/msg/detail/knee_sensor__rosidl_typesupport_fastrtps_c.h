// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice
#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "knee_sensor_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "knee_sensor_interface/msg/detail/knee_sensor__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
bool cdr_serialize_knee_sensor_interface__msg__KneeSensor(
  const knee_sensor_interface__msg__KneeSensor * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
bool cdr_deserialize_knee_sensor_interface__msg__KneeSensor(
  eprosima::fastcdr::Cdr &,
  knee_sensor_interface__msg__KneeSensor * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
size_t get_serialized_size_knee_sensor_interface__msg__KneeSensor(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
size_t max_serialized_size_knee_sensor_interface__msg__KneeSensor(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
bool cdr_serialize_key_knee_sensor_interface__msg__KneeSensor(
  const knee_sensor_interface__msg__KneeSensor * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
size_t get_serialized_size_key_knee_sensor_interface__msg__KneeSensor(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
size_t max_serialized_size_key_knee_sensor_interface__msg__KneeSensor(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_knee_sensor_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, knee_sensor_interface, msg, KneeSensor)();

#ifdef __cplusplus
}
#endif

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
