// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice
#include "knee_sensor_interface/msg/detail/knee_sensor__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `channels`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
knee_sensor_interface__msg__KneeSensor__init(knee_sensor_interface__msg__KneeSensor * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    knee_sensor_interface__msg__KneeSensor__fini(msg);
    return false;
  }
  // angle
  // angular_velocity
  // channels
  if (!rosidl_runtime_c__double__Sequence__init(&msg->channels, 0)) {
    knee_sensor_interface__msg__KneeSensor__fini(msg);
    return false;
  }
  return true;
}

void
knee_sensor_interface__msg__KneeSensor__fini(knee_sensor_interface__msg__KneeSensor * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // angle
  // angular_velocity
  // channels
  rosidl_runtime_c__double__Sequence__fini(&msg->channels);
}

bool
knee_sensor_interface__msg__KneeSensor__are_equal(const knee_sensor_interface__msg__KneeSensor * lhs, const knee_sensor_interface__msg__KneeSensor * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // angle
  if (lhs->angle != rhs->angle) {
    return false;
  }
  // angular_velocity
  if (lhs->angular_velocity != rhs->angular_velocity) {
    return false;
  }
  // channels
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->channels), &(rhs->channels)))
  {
    return false;
  }
  return true;
}

bool
knee_sensor_interface__msg__KneeSensor__copy(
  const knee_sensor_interface__msg__KneeSensor * input,
  knee_sensor_interface__msg__KneeSensor * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // angle
  output->angle = input->angle;
  // angular_velocity
  output->angular_velocity = input->angular_velocity;
  // channels
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->channels), &(output->channels)))
  {
    return false;
  }
  return true;
}

knee_sensor_interface__msg__KneeSensor *
knee_sensor_interface__msg__KneeSensor__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  knee_sensor_interface__msg__KneeSensor * msg = (knee_sensor_interface__msg__KneeSensor *)allocator.allocate(sizeof(knee_sensor_interface__msg__KneeSensor), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(knee_sensor_interface__msg__KneeSensor));
  bool success = knee_sensor_interface__msg__KneeSensor__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
knee_sensor_interface__msg__KneeSensor__destroy(knee_sensor_interface__msg__KneeSensor * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    knee_sensor_interface__msg__KneeSensor__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
knee_sensor_interface__msg__KneeSensor__Sequence__init(knee_sensor_interface__msg__KneeSensor__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  knee_sensor_interface__msg__KneeSensor * data = NULL;

  if (size) {
    data = (knee_sensor_interface__msg__KneeSensor *)allocator.zero_allocate(size, sizeof(knee_sensor_interface__msg__KneeSensor), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = knee_sensor_interface__msg__KneeSensor__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        knee_sensor_interface__msg__KneeSensor__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
knee_sensor_interface__msg__KneeSensor__Sequence__fini(knee_sensor_interface__msg__KneeSensor__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      knee_sensor_interface__msg__KneeSensor__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

knee_sensor_interface__msg__KneeSensor__Sequence *
knee_sensor_interface__msg__KneeSensor__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  knee_sensor_interface__msg__KneeSensor__Sequence * array = (knee_sensor_interface__msg__KneeSensor__Sequence *)allocator.allocate(sizeof(knee_sensor_interface__msg__KneeSensor__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = knee_sensor_interface__msg__KneeSensor__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
knee_sensor_interface__msg__KneeSensor__Sequence__destroy(knee_sensor_interface__msg__KneeSensor__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    knee_sensor_interface__msg__KneeSensor__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
knee_sensor_interface__msg__KneeSensor__Sequence__are_equal(const knee_sensor_interface__msg__KneeSensor__Sequence * lhs, const knee_sensor_interface__msg__KneeSensor__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!knee_sensor_interface__msg__KneeSensor__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
knee_sensor_interface__msg__KneeSensor__Sequence__copy(
  const knee_sensor_interface__msg__KneeSensor__Sequence * input,
  knee_sensor_interface__msg__KneeSensor__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(knee_sensor_interface__msg__KneeSensor);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    knee_sensor_interface__msg__KneeSensor * data =
      (knee_sensor_interface__msg__KneeSensor *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!knee_sensor_interface__msg__KneeSensor__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          knee_sensor_interface__msg__KneeSensor__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!knee_sensor_interface__msg__KneeSensor__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
