// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "knee_sensor_interface/msg/knee_sensor.hpp"


#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__BUILDER_HPP_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "knee_sensor_interface/msg/detail/knee_sensor__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace knee_sensor_interface
{

namespace msg
{

namespace builder
{

class Init_KneeSensor_channels
{
public:
  explicit Init_KneeSensor_channels(::knee_sensor_interface::msg::KneeSensor & msg)
  : msg_(msg)
  {}
  ::knee_sensor_interface::msg::KneeSensor channels(::knee_sensor_interface::msg::KneeSensor::_channels_type arg)
  {
    msg_.channels = std::move(arg);
    return std::move(msg_);
  }

private:
  ::knee_sensor_interface::msg::KneeSensor msg_;
};

class Init_KneeSensor_angular_velocity
{
public:
  explicit Init_KneeSensor_angular_velocity(::knee_sensor_interface::msg::KneeSensor & msg)
  : msg_(msg)
  {}
  Init_KneeSensor_channels angular_velocity(::knee_sensor_interface::msg::KneeSensor::_angular_velocity_type arg)
  {
    msg_.angular_velocity = std::move(arg);
    return Init_KneeSensor_channels(msg_);
  }

private:
  ::knee_sensor_interface::msg::KneeSensor msg_;
};

class Init_KneeSensor_angle
{
public:
  explicit Init_KneeSensor_angle(::knee_sensor_interface::msg::KneeSensor & msg)
  : msg_(msg)
  {}
  Init_KneeSensor_angular_velocity angle(::knee_sensor_interface::msg::KneeSensor::_angle_type arg)
  {
    msg_.angle = std::move(arg);
    return Init_KneeSensor_angular_velocity(msg_);
  }

private:
  ::knee_sensor_interface::msg::KneeSensor msg_;
};

class Init_KneeSensor_header
{
public:
  Init_KneeSensor_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_KneeSensor_angle header(::knee_sensor_interface::msg::KneeSensor::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_KneeSensor_angle(msg_);
  }

private:
  ::knee_sensor_interface::msg::KneeSensor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::knee_sensor_interface::msg::KneeSensor>()
{
  return knee_sensor_interface::msg::builder::Init_KneeSensor_header();
}

}  // namespace knee_sensor_interface

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__BUILDER_HPP_
