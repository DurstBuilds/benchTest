// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "knee_sensor_interface/msg/knee_sensor.h"


#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_H_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'channels'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/KneeSensor in the package knee_sensor_interface.
/**
  * Knee sensor reading
 */
typedef struct knee_sensor_interface__msg__KneeSensor
{
  std_msgs__msg__Header header;
  /// Joint angle in radians
  double angle;
  /// Angular velocity in rad/s
  double angular_velocity;
  /// Optional force/torque or strain channels
  rosidl_runtime_c__double__Sequence channels;
} knee_sensor_interface__msg__KneeSensor;

// Struct for a sequence of knee_sensor_interface__msg__KneeSensor.
typedef struct knee_sensor_interface__msg__KneeSensor__Sequence
{
  knee_sensor_interface__msg__KneeSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} knee_sensor_interface__msg__KneeSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_H_
