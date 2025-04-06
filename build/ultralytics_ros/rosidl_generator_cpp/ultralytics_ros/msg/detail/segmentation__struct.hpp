// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice

#ifndef ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_HPP_
#define ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'image'
// Member 'masks'
#include "sensor_msgs/msg/detail/image__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ultralytics_ros__msg__Segmentation __attribute__((deprecated))
#else
# define DEPRECATED__ultralytics_ros__msg__Segmentation __declspec(deprecated)
#endif

namespace ultralytics_ros
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Segmentation_
{
  using Type = Segmentation_<ContainerAllocator>;

  explicit Segmentation_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    image(_init)
  {
    (void)_init;
  }

  explicit Segmentation_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    image(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _image_type =
    sensor_msgs::msg::Image_<ContainerAllocator>;
  _image_type image;
  using _masks_type =
    std::vector<sensor_msgs::msg::Image_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sensor_msgs::msg::Image_<ContainerAllocator>>>;
  _masks_type masks;
  using _scores_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _scores_type scores;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__image(
    const sensor_msgs::msg::Image_<ContainerAllocator> & _arg)
  {
    this->image = _arg;
    return *this;
  }
  Type & set__masks(
    const std::vector<sensor_msgs::msg::Image_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sensor_msgs::msg::Image_<ContainerAllocator>>> & _arg)
  {
    this->masks = _arg;
    return *this;
  }
  Type & set__scores(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->scores = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ultralytics_ros::msg::Segmentation_<ContainerAllocator> *;
  using ConstRawPtr =
    const ultralytics_ros::msg::Segmentation_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ultralytics_ros::msg::Segmentation_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ultralytics_ros::msg::Segmentation_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ultralytics_ros__msg__Segmentation
    std::shared_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ultralytics_ros__msg__Segmentation
    std::shared_ptr<ultralytics_ros::msg::Segmentation_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Segmentation_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->image != other.image) {
      return false;
    }
    if (this->masks != other.masks) {
      return false;
    }
    if (this->scores != other.scores) {
      return false;
    }
    return true;
  }
  bool operator!=(const Segmentation_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Segmentation_

// alias to use template instance with default allocator
using Segmentation =
  ultralytics_ros::msg::Segmentation_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace ultralytics_ros

#endif  // ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__STRUCT_HPP_
