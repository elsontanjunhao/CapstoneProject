// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ultralytics_ros:msg/Segmentation.idl
// generated code does not contain a copyright notice

#ifndef ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__TRAITS_HPP_
#define ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ultralytics_ros/msg/detail/segmentation__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'image'
// Member 'masks'
#include "sensor_msgs/msg/detail/image__traits.hpp"

namespace ultralytics_ros
{

namespace msg
{

inline void to_flow_style_yaml(
  const Segmentation & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: image
  {
    out << "image: ";
    to_flow_style_yaml(msg.image, out);
    out << ", ";
  }

  // member: masks
  {
    if (msg.masks.size() == 0) {
      out << "masks: []";
    } else {
      out << "masks: [";
      size_t pending_items = msg.masks.size();
      for (auto item : msg.masks) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: scores
  {
    if (msg.scores.size() == 0) {
      out << "scores: []";
    } else {
      out << "scores: [";
      size_t pending_items = msg.scores.size();
      for (auto item : msg.scores) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Segmentation & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: image
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "image:\n";
    to_block_style_yaml(msg.image, out, indentation + 2);
  }

  // member: masks
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.masks.size() == 0) {
      out << "masks: []\n";
    } else {
      out << "masks:\n";
      for (auto item : msg.masks) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: scores
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.scores.size() == 0) {
      out << "scores: []\n";
    } else {
      out << "scores:\n";
      for (auto item : msg.scores) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Segmentation & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace ultralytics_ros

namespace rosidl_generator_traits
{

[[deprecated("use ultralytics_ros::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ultralytics_ros::msg::Segmentation & msg,
  std::ostream & out, size_t indentation = 0)
{
  ultralytics_ros::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ultralytics_ros::msg::to_yaml() instead")]]
inline std::string to_yaml(const ultralytics_ros::msg::Segmentation & msg)
{
  return ultralytics_ros::msg::to_yaml(msg);
}

template<>
inline const char * data_type<ultralytics_ros::msg::Segmentation>()
{
  return "ultralytics_ros::msg::Segmentation";
}

template<>
inline const char * name<ultralytics_ros::msg::Segmentation>()
{
  return "ultralytics_ros/msg/Segmentation";
}

template<>
struct has_fixed_size<ultralytics_ros::msg::Segmentation>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ultralytics_ros::msg::Segmentation>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<ultralytics_ros::msg::Segmentation>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ULTRALYTICS_ROS__MSG__DETAIL__SEGMENTATION__TRAITS_HPP_
