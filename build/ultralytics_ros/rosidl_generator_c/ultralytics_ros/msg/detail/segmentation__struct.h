// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice

#ifndef ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_H_
#define ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_H_

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
// Member 'image'
// Member 'masks'
#include "sensor_msgs/msg/detail/image__struct.h"
// Member 'scores'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Segmentation in the package ultralytics_ros.
typedef struct ultralytics_ros__msg__Segmentation
{
  std_msgs__msg__Header header;
  sensor_msgs__msg__Image image;
  sensor_msgs__msg__Image__Sequence masks;
  rosidl_runtime_c__float__Sequence scores;
} ultralytics_ros__msg__Segmentation;

// Struct for a sequence of ultralytics_ros__msg__Segmentation.
typedef struct ultralytics_ros__msg__Segmentation__Sequence
{
  ultralytics_ros__msg__Segmentation * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ultralytics_ros__msg__Segmentation__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_H_
