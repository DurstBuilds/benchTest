// knee_sensor_interface_node: read a PIC HS-3511-02-0300 latching Hall over
// Raspberry Pi GPIO and publish KneeSensor (bool active) on knee_sensor.
//
// Hardware: PIC HS-3511-02-0300 latching 3-wire Hall (red VSUP, white OUT,
// black GND). Open-drain OUT with external pull-up → active-low.
//
// Wiring (BCM GPIO 25 / physical pin 22):
//   - 220 Ohm series protection resistor to pin 22
//   - 1.2 kOhm pull-up from +3.3 V to the sensor side of the series R
//   - 10 nF ceramic from that same node to GND (~13.3 kHz LPF)
//   - 0.1 uF ceramic at the sensor between +3.3 V and GND
// Process needs access to /dev/gpiochip* (gpio group or udev rule).
// Requires system package libgpiod-dev.
//
// Parameters (TWEAK via launch or ros2 param):
//   gpio_chip         — libgpiod chip name (default gpiochip0)
//   gpio_line         — BCM line number (default 25)
//   active_low        — invert open-drain level (default true)
//   publish_rate_hz   — poll/publish rate (default 50.0)
//   sensor_topic      — publication name (default knee_sensor)

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

#include <gpiod.h>

#include "knee_sensor_interface/msg/knee_sensor.hpp"
#include "rclcpp/rclcpp.hpp"

class KneeSensorInterfaceNode : public rclcpp::Node
{
public:
  KneeSensorInterfaceNode()
  : Node("knee_sensor_interface_node")
  {
    // TWEAK: chip, line, polarity, rate, and topic
    gpio_chip_ = declare_parameter<std::string>("gpio_chip", "gpiochip0");
    gpio_line_ = declare_parameter<int>("gpio_line", 25);
    active_low_ = declare_parameter<bool>("active_low", true);
    const double publish_rate_hz = declare_parameter<double>("publish_rate_hz", 50.0);
    const auto sensor_topic = declare_parameter<std::string>("sensor_topic", "knee_sensor");

    if (gpio_line_ < 0) {
      throw std::runtime_error("gpio_line must be non-negative");
    }
    if (publish_rate_hz <= 0.0) {
      throw std::runtime_error("publish_rate_hz must be positive");
    }

    if (!openGpio()) {
      throw std::runtime_error(
        "Failed to open GPIO '" + gpio_chip_ + "' line " + std::to_string(gpio_line_) +
        ": " + std::strerror(errno));
    }

    publisher_ = create_publisher<knee_sensor_interface::msg::KneeSensor>(
      sensor_topic, rclcpp::SensorDataQoS());

    const int raw = gpiod_line_get_value(line_);
    if (raw < 0) {
      closeGpio();
      throw std::runtime_error(
        "Failed to read initial GPIO value: " + std::string(std::strerror(errno)));
    }
    const bool active = rawToActive(raw);

    const auto period = std::chrono::duration<double>(1.0 / publish_rate_hz);
    timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      [this]() { onTimer(); });

    RCLCPP_INFO(
      get_logger(),
      "Publishing HS-3511 latch on '%s' at %.1f Hz via %s line %d "
      "(active_low=%s, initial active=%s)",
      sensor_topic.c_str(), publish_rate_hz, gpio_chip_.c_str(), gpio_line_,
      active_low_ ? "true" : "false", active ? "true" : "false");
  }

  ~KneeSensorInterfaceNode() override
  {
    closeGpio();
  }

private:
  // Map raw line level (0/1) to latch-active boolean using active_low_.
  bool rawToActive(int raw) const
  {
    const bool level_high = (raw != 0);
    return active_low_ ? !level_high : level_high;
  }

  // Open gpiochip and request the configured line as an input.
  bool openGpio()
  {
    chip_ = gpiod_chip_open_by_name(gpio_chip_.c_str());
    if (chip_ == nullptr) {
      return false;
    }

    line_ = gpiod_chip_get_line(chip_, static_cast<unsigned int>(gpio_line_));
    if (line_ == nullptr) {
      closeGpio();
      return false;
    }

    // External pull-up is on the board; do not configure bias here.
    if (gpiod_line_request_input(line_, "knee_sensor_interface") < 0) {
      closeGpio();
      return false;
    }

    return true;
  }

  void closeGpio()
  {
    if (line_ != nullptr) {
      gpiod_line_release(line_);
      line_ = nullptr;
    }
    if (chip_ != nullptr) {
      gpiod_chip_close(chip_);
      chip_ = nullptr;
    }
  }

  void onTimer()
  {
    const int raw = gpiod_line_get_value(line_);
    if (raw < 0) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "GPIO read failed: %s", std::strerror(errno));
      return;
    }

    knee_sensor_interface::msg::KneeSensor msg;
    msg.header.stamp = now();
    msg.header.frame_id = "";
    msg.active = rawToActive(raw);
    publisher_->publish(msg);

    RCLCPP_DEBUG(
      get_logger(), "Hall: raw=%d active=%s", raw, msg.active ? "true" : "false");
  }

  std::string gpio_chip_;
  int gpio_line_ {25};
  bool active_low_ {true};
  gpiod_chip * chip_ {nullptr};
  gpiod_line * line_ {nullptr};
  rclcpp::Publisher<knee_sensor_interface::msg::KneeSensor>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<KneeSensorInterfaceNode>();
    rclcpp::spin(node);
  } catch (const std::exception & ex) {
    fprintf(stderr, "knee_sensor_interface_node failed to start: %s\n", ex.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
