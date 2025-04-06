// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice

#ifndef ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__BUILDER_HPP_
#define ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ultralytics_ros/msg/detail/segmentation__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ultralytics_ros
{

namespace msg
{

namespace builder
{

class Init_Segmentation_scores
{
public:
  explicit Init_Segmentation_scores(::ultralytics_ros::msg::Segmentation & msg)
  : msg_(msg)
  {}
  ::ultralytics_ros::msg::Segmentation scores(::ultralytics_ros::msg::Segmentation::_scores_type arg)
  {
    msg_.scores = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ultralytics_ros::msg::Segmentation msg_;
};

class Init_Segmentation_masks
{
public:
  explicit Init_Segmentation_masks(::ultralytics_ros::msg::Segmentation & msg)
  : msg_(msg)
  {}
  Init_Segmentation_scores masks(::ultralytics_ros::msg::Segmentation::_masks_type arg)
  {
    msg_.masks = std::move(arg);
    return Init_Segmentation_scores(msg_);
  }

private:
  ::ultralytics_ros::msg::Segmentation msg_;
};

class Init_Segmentation_image
{
public:
  explicit Init_Segmentation_image(::ultralytics_ros::msg::Segmentation & msg)
  : msg_(msg)
  {}
  Init_Segmentation_masks image(::ultralytics_ros::msg::Segmentation::_image_type arg)
  {
    msg_.image = std::move(arg);
    return Init_Segmentation_masks(msg_);
  }

private:
  ::ultralytics_ros::msg::Segmentation msg_;
};

class Init_Segmentation_header
{
public:
  Init_Segmentation_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Segmentation_image header(::ultralytics_ros::msg::Segmentation::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Segmentation_image(msg_);
  }

private:
  ::ultralytics_ros::msg::Segmentation msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ultralytics_ros::msg::Segmentation>()
{
  return ultralytics_ros::msg::builder::Init_Segmentation_header();
}

}  // namespace ultralytics_ros

#endif  // ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__BUILDER_HPP_
