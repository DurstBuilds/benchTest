// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from knee_sensor_interface:msg/KneeSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "knee_sensor_interface/msg/knee_sensor.hpp"


#ifndef KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_HPP_
#define KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__knee_sensor_interface__msg__KneeSensor __attribute__((deprecated))
#else
# define DEPRECATED__knee_sensor_interface__msg__KneeSensor __declspec(deprecated)
#endif

namespace knee_sensor_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct KneeSensor_
{
  using Type = KneeSensor_<ContainerAllocator>;

  explicit KneeSensor_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->angle = 0.0;
      this->angular_velocity = 0.0;
    }
  }

  explicit KneeSensor_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->angle = 0.0;
      this->angular_velocity = 0.0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _angle_type =
    double;
  _angle_type angle;
  using _angular_velocity_type =
    double;
  _angular_velocity_type angular_velocity;
  using _channels_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _channels_type channels;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__angle(
    const double & _arg)
  {
    this->angle = _arg;
    return *this;
  }
  Type & set__angular_velocity(
    const double & _arg)
  {
    this->angular_velocity = _arg;
    return *this;
  }
  Type & set__channels(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->channels = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> *;
  using ConstRawPtr =
    const knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__knee_sensor_interface__msg__KneeSensor
    std::shared_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__knee_sensor_interface__msg__KneeSensor
    std::shared_ptr<knee_sensor_interface::msg::KneeSensor_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const KneeSensor_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->angle != other.angle) {
      return false;
    }
    if (this->angular_velocity != other.angular_velocity) {
      return false;
    }
    if (this->channels != other.channels) {
      return false;
    }
    return true;
  }
  bool operator!=(const KneeSensor_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct KneeSensor_

// alias to use template instance with default allocator
using KneeSensor =
  knee_sensor_interface::msg::KneeSensor_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace knee_sensor_interface

#endif  // KNEE_SENSOR_INTERFACE__MSG__DETAIL__KNEE_SENSOR__STRUCT_HPP_
