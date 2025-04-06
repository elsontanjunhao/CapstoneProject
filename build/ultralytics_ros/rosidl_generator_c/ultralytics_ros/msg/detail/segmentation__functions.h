// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice

#ifndef ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__FUNCTIONS_H_
#define ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "ultralytics_ros/msg/rosidl_generator_c__visibility_control.h"

#include "ultralytics_ros/msg/detail/segmentation__struct.h"

/// Initialize msg/Segmentation message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * ultralytics_ros__msg__Segmentation
 * )) before or use
 * ultralytics_ros__msg__Segmentation__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__init(ultralytics_ros__msg__Segmentation * msg);

/// Finalize msg/Segmentation message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
void
ultralytics_ros__msg__Segmentation__fini(ultralytics_ros__msg__Segmentation * msg);

/// Create msg/Segmentation message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * ultralytics_ros__msg__Segmentation__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
ultralytics_ros__msg__Segmentation *
ultralytics_ros__msg__Segmentation__create();

/// Destroy msg/Segmentation message.
/**
 * It calls
 * ultralytics_ros__msg__Segmentation__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
void
ultralytics_ros__msg__Segmentation__destroy(ultralytics_ros__msg__Segmentation * msg);

/// Check for msg/Segmentation message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__are_equal(const ultralytics_ros__msg__Segmentation * lhs, const ultralytics_ros__msg__Segmentation * rhs);

/// Copy a msg/Segmentation message.
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
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__copy(
  const ultralytics_ros__msg__Segmentation * input,
  ultralytics_ros__msg__Segmentation * output);

/// Initialize array of msg/Segmentation messages.
/**
 * It allocates the memory for the number of elements and calls
 * ultralytics_ros__msg__Segmentation__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__Sequence__init(ultralytics_ros__msg__Segmentation__Sequence * array, size_t size);

/// Finalize array of msg/Segmentation messages.
/**
 * It calls
 * ultralytics_ros__msg__Segmentation__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
void
ultralytics_ros__msg__Segmentation__Sequence__fini(ultralytics_ros__msg__Segmentation__Sequence * array);

/// Create array of msg/Segmentation messages.
/**
 * It allocates the memory for the array and calls
 * ultralytics_ros__msg__Segmentation__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
ultralytics_ros__msg__Segmentation__Sequence *
ultralytics_ros__msg__Segmentation__Sequence__create(size_t size);

/// Destroy array of msg/Segmentation messages.
/**
 * It calls
 * ultralytics_ros__msg__Segmentation__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
void
ultralytics_ros__msg__Segmentation__Sequence__destroy(ultralytics_ros__msg__Segmentation__Sequence * array);

/// Check for msg/Segmentation message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__Sequence__are_equal(const ultralytics_ros__msg__Segmentation__Sequence * lhs, const ultralytics_ros__msg__Segmentation__Sequence * rhs);

/// Copy an array of msg/Segmentation messages.
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
ROSIDL_GENERATOR_C_PUBLIC_ultralytics_ros
bool
ultralytics_ros__msg__Segmentation__Sequence__copy(
  const ultralytics_ros__msg__Segmentation__Sequence * input,
  ultralytics_ros__msg__Segmentation__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__FUNCTIONS_H_
