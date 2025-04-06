// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice
#include "ultralytics_ros/msg/detail/segmentation__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `image`
// Member `masks`
#include "sensor_msgs/msg/detail/image__functions.h"
// Member `scores`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
ultralytics_ros__msg__Segmentation__init(ultralytics_ros__msg__Segmentation * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    ultralytics_ros__msg__Segmentation__fini(msg);
    return false;
  }
  // image
  if (!sensor_msgs__msg__Image__init(&msg->image)) {
    ultralytics_ros__msg__Segmentation__fini(msg);
    return false;
  }
  // masks
  if (!sensor_msgs__msg__Image__Sequence__init(&msg->masks, 0)) {
    ultralytics_ros__msg__Segmentation__fini(msg);
    return false;
  }
  // scores
  if (!rosidl_runtime_c__float__Sequence__init(&msg->scores, 0)) {
    ultralytics_ros__msg__Segmentation__fini(msg);
    return false;
  }
  return true;
}

void
ultralytics_ros__msg__Segmentation__fini(ultralytics_ros__msg__Segmentation * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // image
  sensor_msgs__msg__Image__fini(&msg->image);
  // masks
  sensor_msgs__msg__Image__Sequence__fini(&msg->masks);
  // scores
  rosidl_runtime_c__float__Sequence__fini(&msg->scores);
}

bool
ultralytics_ros__msg__Segmentation__are_equal(const ultralytics_ros__msg__Segmentation * lhs, const ultralytics_ros__msg__Segmentation * rhs)
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
  // image
  if (!sensor_msgs__msg__Image__are_equal(
      &(lhs->image), &(rhs->image)))
  {
    return false;
  }
  // masks
  if (!sensor_msgs__msg__Image__Sequence__are_equal(
      &(lhs->masks), &(rhs->masks)))
  {
    return false;
  }
  // scores
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->scores), &(rhs->scores)))
  {
    return false;
  }
  return true;
}

bool
ultralytics_ros__msg__Segmentation__copy(
  const ultralytics_ros__msg__Segmentation * input,
  ultralytics_ros__msg__Segmentation * output)
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
  // image
  if (!sensor_msgs__msg__Image__copy(
      &(input->image), &(output->image)))
  {
    return false;
  }
  // masks
  if (!sensor_msgs__msg__Image__Sequence__copy(
      &(input->masks), &(output->masks)))
  {
    return false;
  }
  // scores
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->scores), &(output->scores)))
  {
    return false;
  }
  return true;
}

ultralytics_ros__msg__Segmentation *
ultralytics_ros__msg__Segmentation__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ultralytics_ros__msg__Segmentation * msg = (ultralytics_ros__msg__Segmentation *)allocator.allocate(sizeof(ultralytics_ros__msg__Segmentation), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ultralytics_ros__msg__Segmentation));
  bool success = ultralytics_ros__msg__Segmentation__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ultralytics_ros__msg__Segmentation__destroy(ultralytics_ros__msg__Segmentation * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ultralytics_ros__msg__Segmentation__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ultralytics_ros__msg__Segmentation__Sequence__init(ultralytics_ros__msg__Segmentation__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ultralytics_ros__msg__Segmentation * data = NULL;

  if (size) {
    data = (ultralytics_ros__msg__Segmentation *)allocator.zero_allocate(size, sizeof(ultralytics_ros__msg__Segmentation), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ultralytics_ros__msg__Segmentation__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ultralytics_ros__msg__Segmentation__fini(&data[i - 1]);
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
ultralytics_ros__msg__Segmentation__Sequence__fini(ultralytics_ros__msg__Segmentation__Sequence * array)
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
      ultralytics_ros__msg__Segmentation__fini(&array->data[i]);
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

ultralytics_ros__msg__Segmentation__Sequence *
ultralytics_ros__msg__Segmentation__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ultralytics_ros__msg__Segmentation__Sequence * array = (ultralytics_ros__msg__Segmentation__Sequence *)allocator.allocate(sizeof(ultralytics_ros__msg__Segmentation__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ultralytics_ros__msg__Segmentation__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ultralytics_ros__msg__Segmentation__Sequence__destroy(ultralytics_ros__msg__Segmentation__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ultralytics_ros__msg__Segmentation__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ultralytics_ros__msg__Segmentation__Sequence__are_equal(const ultralytics_ros__msg__Segmentation__Sequence * lhs, const ultralytics_ros__msg__Segmentation__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ultralytics_ros__msg__Segmentation__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ultralytics_ros__msg__Segmentation__Sequence__copy(
  const ultralytics_ros__msg__Segmentation__Sequence * input,
  ultralytics_ros__msg__Segmentation__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ultralytics_ros__msg__Segmentation);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ultralytics_ros__msg__Segmentation * data =
      (ultralytics_ros__msg__Segmentation *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ultralytics_ros__msg__Segmentation__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ultralytics_ros__msg__Segmentation__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ultralytics_ros__msg__Segmentation__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
