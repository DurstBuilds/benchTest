// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "knee_sensor_interface/msg/knee_sensor.hpp"


#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__TRAITS_HPP_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "knee_sensor_interface/msg/detail/knee_sensor__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace knee_sensor_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const KneeSensor & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: angle
  {
    out << "angle: ";
    rosidl_generator_traits::value_to_yaml(msg.angle, out);
    out << ", ";
  }

  // member: angular_velocity
  {
    out << "angular_velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.angular_velocity, out);
    out << ", ";
  }

  // member: channels
  {
    if (msg.channels.size() == 0) {
      out << "channels: []";
    } else {
      out << "channels: [";
      size_t pending_items = msg.channels.size();
      for (auto item : msg.channels) {
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
  const KneeSensor & msg,
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

  // member: angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "angle: ";
    rosidl_generator_traits::value_to_yaml(msg.angle, out);
    out << "\n";
  }

  // member: angular_velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "angular_velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.angular_velocity, out);
    out << "\n";
  }

  // member: channels
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.channels.size() == 0) {
      out << "channels: []\n";
    } else {
      out << "channels:\n";
      for (auto item : msg.channels) {
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

inline std::string to_yaml(const KneeSensor & msg, bool use_flow_style = false)
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

}  // namespace knee_sensor_interface

namespace rosidl_generator_traits
{

[[deprecated("use knee_sensor_interface::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const knee_sensor_interface::msg::KneeSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  knee_sensor_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use knee_sensor_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const knee_sensor_interface::msg::KneeSensor & msg)
{
  return knee_sensor_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<knee_sensor_interface::msg::KneeSensor>()
{
  return "knee_sensor_interface::msg::KneeSensor";
}

template<>
inline const char * name<knee_sensor_interface::msg::KneeSensor>()
{
  return "knee_sensor_interface/msg/KneeSensor";
}

template<>
struct has_fixed_size<knee_sensor_interface::msg::KneeSensor>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<knee_sensor_interface::msg::KneeSensor>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<knee_sensor_interface::msg::KneeSensor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__TRAITS_HPP_
