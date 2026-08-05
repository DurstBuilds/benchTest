// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "knee_sensor_interface/msg/knee_sensor.h"


#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__FUNCTIONS_H_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "knee_sensor_interface/msg/rosidl_generator_c__visibility_control.h"

#include "knee_sensor_interface/msg/detail/knee_sensor__struct.h"

/// Initialize msg/KneeSensor message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * knee_sensor_interface__msg__KneeSensor
 * )) before or use
 * knee_sensor_interface__msg__KneeSensor__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__init(knee_sensor_interface__msg__KneeSensor * msg);

/// Finalize msg/KneeSensor message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
void
knee_sensor_interface__msg__KneeSensor__fini(knee_sensor_interface__msg__KneeSensor * msg);

/// Create msg/KneeSensor message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * knee_sensor_interface__msg__KneeSensor__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
knee_sensor_interface__msg__KneeSensor *
knee_sensor_interface__msg__KneeSensor__create(void);

/// Destroy msg/KneeSensor message.
/**
 * It calls
 * knee_sensor_interface__msg__KneeSensor__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
void
knee_sensor_interface__msg__KneeSensor__destroy(knee_sensor_interface__msg__KneeSensor * msg);

/// Check for msg/KneeSensor message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__are_equal(const knee_sensor_interface__msg__KneeSensor * lhs, const knee_sensor_interface__msg__KneeSensor * rhs);

/// Copy a msg/KneeSensor message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__copy(
  const knee_sensor_interface__msg__KneeSensor * input,
  knee_sensor_interface__msg__KneeSensor * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
const rosidl_type_hash_t *
knee_sensor_interface__msg__KneeSensor__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
const rosidl_runtime_c__type_description__TypeDescription *
knee_sensor_interface__msg__KneeSensor__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
const rosidl_runtime_c__type_description__TypeSource *
knee_sensor_interface__msg__KneeSensor__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
const rosidl_runtime_c__type_description__TypeSource__Sequence *
knee_sensor_interface__msg__KneeSensor__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/KneeSensor messages.
/**
 * It allocates the memory for the number of elements and calls
 * knee_sensor_interface__msg__KneeSensor__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__Sequence__init(knee_sensor_interface__msg__KneeSensor__Sequence * array, size_t size);

/// Finalize array of msg/KneeSensor messages.
/**
 * It calls
 * knee_sensor_interface__msg__KneeSensor__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
void
knee_sensor_interface__msg__KneeSensor__Sequence__fini(knee_sensor_interface__msg__KneeSensor__Sequence * array);

/// Create array of msg/KneeSensor messages.
/**
 * It allocates the memory for the array and calls
 * knee_sensor_interface__msg__KneeSensor__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
knee_sensor_interface__msg__KneeSensor__Sequence *
knee_sensor_interface__msg__KneeSensor__Sequence__create(size_t size);

/// Destroy array of msg/KneeSensor messages.
/**
 * It calls
 * knee_sensor_interface__msg__KneeSensor__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
void
knee_sensor_interface__msg__KneeSensor__Sequence__destroy(knee_sensor_interface__msg__KneeSensor__Sequence * array);

/// Check for msg/KneeSensor message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__Sequence__are_equal(const knee_sensor_interface__msg__KneeSensor__Sequence * lhs, const knee_sensor_interface__msg__KneeSensor__Sequence * rhs);

/// Copy an array of msg/KneeSensor messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_knee_sensor_interface
bool
knee_sensor_interface__msg__KneeSensor__Sequence__copy(
  const knee_sensor_interface__msg__KneeSensor__Sequence * input,
  knee_sensor_interface__msg__KneeSensor__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__FUNCTIONS_H_
