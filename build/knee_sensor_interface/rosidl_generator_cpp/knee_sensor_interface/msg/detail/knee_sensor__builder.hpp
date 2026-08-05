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

class Init_KneeSensor_active
{
public:
  explicit Init_KneeSensor_active(::knee_sensor_interface::msg::KneeSensor & msg)
  : msg_(msg)
  {}
  ::knee_sensor_interface::msg::KneeSensor active(::knee_sensor_interface::msg::KneeSensor::_active_type arg)
  {
    msg_.active = std::move(arg);
    return std::move(msg_);
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
  Init_KneeSensor_active header(::knee_sensor_interface::msg::KneeSensor::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_KneeSensor_active(msg_);
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
