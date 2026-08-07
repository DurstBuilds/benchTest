// calculate_dead_zone: measure angular dead-zone between Hall latch edges.
//
// Subscribes to encoder_angle (Float64, rad) and knee_sensor (KneeSensor).
// On each ON↔OFF latch transition after the first, publishes the absolute
// shortest-arc angle delta since the previous transition on dead_zone.
//
// Parameters (TWEAK via launch or ros2 param):
//   angle_topic       — encoder subscription (default encoder_angle)
//   sensor_topic      — hall subscription (default knee_sensor)
//   dead_zone_topic   — publication name (default dead_zone)

#include <cmath>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>

#include "knee_sensor_interface/msg/knee_sensor.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

namespace
{
// Absolute shortest-arc difference between two angles in radians.
double shortestArcAbs(double from_rad, double to_rad)
{
  const double d = to_rad - from_rad;
  return std::abs(std::atan2(std::sin(d), std::cos(d)));
}
}  // namespace

class CalculateDeadZoneNode : public rclcpp::Node
{
public:
  CalculateDeadZoneNode()
  : Node("calculate_dead_zone")
  {
    // TWEAK: input and output topic names
    const auto angle_topic = declare_parameter<std::string>("angle_topic", "encoder_angle");
    const auto sensor_topic = declare_parameter<std::string>("sensor_topic", "knee_sensor");
    const auto dead_zone_topic =
      declare_parameter<std::string>("dead_zone_topic", "dead_zone");

    // Encoder is high-rate best-effort; latch events are reliable (see knee node).
    const auto encoder_qos = rclcpp::SensorDataQoS();
    const auto latch_qos = rclcpp::QoS(10).reliable();

    angle_sub_ = create_subscription<std_msgs::msg::Float64>(
      angle_topic, encoder_qos,
      [this](const std_msgs::msg::Float64::SharedPtr msg) { onAngle(msg); });

    sensor_sub_ = create_subscription<knee_sensor_interface::msg::KneeSensor>(
      sensor_topic, latch_qos,
      [this](const knee_sensor_interface::msg::KneeSensor::SharedPtr msg) {
        onSensor(msg);
      });

    publisher_ = create_publisher<std_msgs::msg::Float64>(dead_zone_topic, latch_qos);

    RCLCPP_INFO(
      get_logger(),
      "Dead-zone: angle='%s', sensor='%s', publish='%s'",
      angle_topic.c_str(), sensor_topic.c_str(), dead_zone_topic.c_str());
  }

private:
  void onAngle(const std_msgs::msg::Float64::SharedPtr msg)
  {
    latest_angle_rad_ = msg->data;
  }

  void onSensor(const knee_sensor_interface::msg::KneeSensor::SharedPtr msg)
  {
    if (!latest_angle_rad_.has_value()) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000,
        "Ignoring latch edge: no encoder_angle sample yet");
      return;
    }

    if (!prev_active_.has_value()) {
      prev_active_ = msg->active;
      prev_transition_angle_rad_ = *latest_angle_rad_;
      RCLCPP_INFO(
        get_logger(),
        "Armed dead-zone at angle=%.6f rad (active=%s)",
        *prev_transition_angle_rad_, msg->active ? "true" : "false");
      return;
    }

    if (msg->active == *prev_active_) {
      return;
    }

    const double current_angle = *latest_angle_rad_;
    const double dead_zone = shortestArcAbs(*prev_transition_angle_rad_, current_angle);
    const char * direction = msg->active ? "OFF->ON" : "ON->OFF";

    std_msgs::msg::Float64 out;
    out.data = dead_zone;
    publisher_->publish(out);

    RCLCPP_INFO(
      get_logger(),
      "Dead-zone %s: %.6f rad (from %.6f to %.6f)",
      direction, dead_zone, *prev_transition_angle_rad_, current_angle);

    prev_active_ = msg->active;
    prev_transition_angle_rad_ = current_angle;
  }

  std::optional<double> latest_angle_rad_;
  std::optional<bool> prev_active_;
  std::optional<double> prev_transition_angle_rad_;

  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr angle_sub_;
  rclcpp::Subscription<knee_sensor_interface::msg::KneeSensor>::SharedPtr sensor_sub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<CalculateDeadZoneNode>();
    rclcpp::spin(node);
  } catch (const std::exception & ex) {
    fprintf(stderr, "calculate_dead_zone failed to start: %s\n", ex.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
