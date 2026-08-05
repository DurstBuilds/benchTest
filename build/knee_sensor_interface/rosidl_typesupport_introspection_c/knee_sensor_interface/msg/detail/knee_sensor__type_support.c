// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "knee_sensor_interface/msg/detail/knee_sensor__rosidl_typesupport_introspection_c.h"
#include "knee_sensor_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "knee_sensor_interface/msg/detail/knee_sensor__functions.h"
#include "knee_sensor_interface/msg/detail/knee_sensor__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `channels`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  knee_sensor_interface__msg__KneeSensor__init(message_memory);
}

void knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_fini_function(void * message_memory)
{
  knee_sensor_interface__msg__KneeSensor__fini(message_memory);
}

size_t knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__size_function__KneeSensor__channels(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_const_function__KneeSensor__channels(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_function__KneeSensor__channels(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__fetch_function__KneeSensor__channels(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_const_function__KneeSensor__channels(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__assign_function__KneeSensor__channels(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_function__KneeSensor__channels(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__resize_function__KneeSensor__channels(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_member_array[4] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(knee_sensor_interface__msg__KneeSensor, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "angle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(knee_sensor_interface__msg__KneeSensor, angle),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "angular_velocity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(knee_sensor_interface__msg__KneeSensor, angular_velocity),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "channels",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(knee_sensor_interface__msg__KneeSensor, channels),  // bytes offset in struct
    NULL,  // default value
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__size_function__KneeSensor__channels,  // size() function pointer
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_const_function__KneeSensor__channels,  // get_const(index) function pointer
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__get_function__KneeSensor__channels,  // get(index) function pointer
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__fetch_function__KneeSensor__channels,  // fetch(index, &value) function pointer
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__assign_function__KneeSensor__channels,  // assign(index, value) function pointer
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__resize_function__KneeSensor__channels  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_members = {
  "knee_sensor_interface__msg",  // message namespace
  "KneeSensor",  // message name
  4,  // number of fields
  sizeof(knee_sensor_interface__msg__KneeSensor),
  false,  // has_any_key_member_
  knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_member_array,  // message members
  knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_type_support_handle = {
  0,
  &knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_members,
  get_message_typesupport_handle_function,
  &knee_sensor_interface__msg__KneeSensor__get_type_hash,
  &knee_sensor_interface__msg__KneeSensor__get_type_description,
  &knee_sensor_interface__msg__KneeSensor__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_knee_sensor_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, knee_sensor_interface, msg, KneeSensor)() {
  knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_type_support_handle.typesupport_identifier) {
    knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &knee_sensor_interface__msg__KneeSensor__rosidl_typesupport_introspection_c__KneeSensor_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
