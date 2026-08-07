// knee_sensor_interface_node: read a PIC HS-3511-02-0300 latching Hall over
// Raspberry Pi GPIO (libgpiod v2) and publish KneeSensor (bool active) on
// knee_sensor. Uses both-edge detection (not polling).
//
// Hardware: PIC HS-3511-02-0300 latching 3-wire Hall (red VSUP, white OUT,
// black GND). Open-drain OUT with external pull-up → active-low.
//
// Wiring (BCM GPIO 23 / physical pin 16):
//   - Signal → BCM GPIO 23 via 220 Ohm series protection resistor
//   - 1.2 kOhm pull-up from +3.3 V to the sensor side of the series R
//   - 10 nF ceramic from that same node to GND (~13.3 kHz LPF)
//   - 0.1 uF ceramic at the sensor between +3.3 V and GND
// Process needs access to /dev/gpiochip* (gpio group or udev rule).
// Requires libgpiod v2 (libgpiod-dev with API >= 2.0).
//
// Parameters (TWEAK via launch or ros2 param):
//   gpio_chip         — chip device path (default /dev/gpiochip0)
//   gpio_line         — BCM line number (default 23)
//   active_low        — invert open-drain level (default true)
//   sensor_topic      — publication name (default knee_sensor)

#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include <gpiod.h>

#include "knee_sensor_interface/msg/knee_sensor.hpp"
#include "rclcpp/rclcpp.hpp"

namespace
{
constexpr int64_t kEdgeWaitTimeoutNs = 100000000;  // 100 ms; allows clean shutdown
constexpr size_t kEdgeEventBufSize = 16;

// Resolve "gpiochip0" → "/dev/gpiochip0"; leave absolute paths unchanged.
std::string resolveChipPath(const std::string & chip)
{
  if (chip.empty()) {
    return "/dev/gpiochip0";
  }
  if (chip.front() == '/') {
    return chip;
  }
  return "/dev/" + chip;
}
}  // namespace

class KneeSensorInterfaceNode : public rclcpp::Node
{
public:
  KneeSensorInterfaceNode()
  : Node("knee_sensor_interface_node")
  {
    // TWEAK: chip path, line, polarity, and topic
    gpio_chip_ = resolveChipPath(
      declare_parameter<std::string>("gpio_chip", "/dev/gpiochip0"));
    gpio_line_ = declare_parameter<int>("gpio_line", 23);
    active_low_ = declare_parameter<bool>("active_low", true);
    const auto sensor_topic = declare_parameter<std::string>("sensor_topic", "knee_sensor");

    if (gpio_line_ < 0) {
      throw std::runtime_error("gpio_line must be non-negative");
    }

    if (!openGpio()) {
      throw std::runtime_error(
        "Failed to open GPIO '" + gpio_chip_ + "' line " + std::to_string(gpio_line_) +
        ": " + std::strerror(errno));
    }

    // Reliable QoS so ros2 topic echo (and most tools) can see latch events.
    publisher_ = create_publisher<knee_sensor_interface::msg::KneeSensor>(
      sensor_topic, rclcpp::QoS(10).reliable());

    // Publish current latch state once, then stream on rising/falling edges.
    const int initial_active = readLogicalActive();
    if (initial_active < 0) {
      closeGpio();
      throw std::runtime_error(
        "Failed to read initial GPIO value: " + std::string(std::strerror(errno)));
    }
    last_active_ = (initial_active != 0);
    publishActive(last_active_);

    running_ = true;
    watcher_ = std::thread([this]() { watchEdges(); });

    RCLCPP_INFO(
      get_logger(),
      "Publishing HS-3511 latch on '%s' via %s line %d "
      "(libgpiod v2 edge detection, active_low=%s, initial active=%s)",
      sensor_topic.c_str(), gpio_chip_.c_str(), gpio_line_,
      active_low_ ? "true" : "false", last_active_ ? "true" : "false");
  }

  ~KneeSensorInterfaceNode() override
  {
    running_ = false;
    if (watcher_.joinable()) {
      watcher_.join();
    }
    closeGpio();
  }

private:
  // Map libgpiod logical edge (after active-low setting) to latch-active.
  bool edgeToActive(enum gpiod_edge_event_type type) const
  {
    // With active_low configured on the request, rising = logical active.
    return type == GPIOD_EDGE_EVENT_RISING_EDGE;
  }

  void publishActive(bool active)
  {
    knee_sensor_interface::msg::KneeSensor msg;
    msg.header.stamp = now();
    msg.header.frame_id = "";
    msg.active = active;
    publisher_->publish(msg);
    last_active_ = active;
  }

  // Publish only when logical level differs from the last published state.
  void publishIfChanged(bool active, const char * reason)
  {
    if (active == last_active_) {
      return;
    }
    publishActive(active);
    RCLCPP_INFO(
      get_logger(), "Hall %s: active=%s", reason, active ? "true" : "false");
  }

  // Open chip and request BCM line as input with both-edge detection (libgpiod v2).
  bool openGpio()
  {
    gpiod_chip * chip = gpiod_chip_open(gpio_chip_.c_str());
    if (chip == nullptr) {
      return false;
    }

    gpiod_line_settings * settings = gpiod_line_settings_new();
    if (settings == nullptr) {
      gpiod_chip_close(chip);
      return false;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_BOTH);
    gpiod_line_settings_set_active_low(settings, active_low_);
    // External pull-up is on the board; do not enable internal bias.
    gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_DISABLED);

    gpiod_line_config * line_cfg = gpiod_line_config_new();
    if (line_cfg == nullptr) {
      gpiod_line_settings_free(settings);
      gpiod_chip_close(chip);
      return false;
    }

    const unsigned int offset = static_cast<unsigned int>(gpio_line_);
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) < 0) {
      gpiod_line_config_free(line_cfg);
      gpiod_line_settings_free(settings);
      gpiod_chip_close(chip);
      return false;
    }

    gpiod_request_config * req_cfg = gpiod_request_config_new();
    if (req_cfg == nullptr) {
      gpiod_line_config_free(line_cfg);
      gpiod_line_settings_free(settings);
      gpiod_chip_close(chip);
      return false;
    }
    gpiod_request_config_set_consumer(req_cfg, "knee_sensor_interface");

    request_ = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    if (request_ == nullptr) {
      return false;
    }

    event_buffer_ = gpiod_edge_event_buffer_new(kEdgeEventBufSize);
    if (event_buffer_ == nullptr) {
      gpiod_line_request_release(request_);
      request_ = nullptr;
      return false;
    }

    return true;
  }

  void closeGpio()
  {
    if (event_buffer_ != nullptr) {
      gpiod_edge_event_buffer_free(event_buffer_);
      event_buffer_ = nullptr;
    }
    if (request_ != nullptr) {
      gpiod_line_request_release(request_);
      request_ = nullptr;
    }
  }

  // Read current logical line level (1=active, 0=inactive); returns -1 on error.
  // Uses request active_low, so ACTIVE means latch ON for the Hall.
  int readLogicalActive()
  {
    const enum gpiod_line_value value =
      gpiod_line_request_get_value(request_, static_cast<unsigned int>(gpio_line_));
    if (value == GPIOD_LINE_VALUE_ERROR) {
      return -1;
    }
    return value == GPIOD_LINE_VALUE_ACTIVE ? 1 : 0;
  }

  // Re-read logical level and publish if it drifted without an edge event.
  void catchMissedLevelChange()
  {
    const int active = readLogicalActive();
    if (active < 0) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000,
        "GPIO level read failed: %s", std::strerror(errno));
      return;
    }
    publishIfChanged(active != 0, "level");
  }

  // Block on GPIO edges in a background thread; publish on each transition.
  void watchEdges()
  {
    while (running_ && rclcpp::ok()) {
      const int ready = gpiod_line_request_wait_edge_events(request_, kEdgeWaitTimeoutNs);
      if (ready < 0) {
        if (errno == EINTR) {
          continue;
        }
        RCLCPP_ERROR(
          get_logger(), "GPIO edge wait failed: %s", std::strerror(errno));
        break;
      }
      if (ready == 0) {
        // Timeout: recover from missed edges (slow edges / driver quirks).
        catchMissedLevelChange();
        continue;
      }

      // GPIO edge events are read here (libgpiod v2).
      const int num_events = gpiod_line_request_read_edge_events(
        request_, event_buffer_, kEdgeEventBufSize);
      if (num_events < 0) {
        RCLCPP_ERROR(
          get_logger(), "GPIO edge read failed: %s", std::strerror(errno));
        break;
      }

      for (int i = 0; i < num_events; ++i) {
        gpiod_edge_event * event = gpiod_edge_event_buffer_get_event(event_buffer_, i);
        if (event == nullptr) {
          continue;
        }
        const enum gpiod_edge_event_type type = gpiod_edge_event_get_event_type(event);
        publishIfChanged(
          edgeToActive(type),
          type == GPIOD_EDGE_EVENT_RISING_EDGE ? "edge-rising" : "edge-falling");
      }

      // Prefer the actual line level after draining the event queue.
      catchMissedLevelChange();
    }
  }

  std::string gpio_chip_;
  int gpio_line_ {23};
  bool active_low_ {true};
  bool last_active_ {false};

  gpiod_line_request * request_ {nullptr};
  gpiod_edge_event_buffer * event_buffer_ {nullptr};

  std::atomic<bool> running_ {false};
  std::thread watcher_;

  rclcpp::Publisher<knee_sensor_interface::msg::KneeSensor>::SharedPtr publisher_;
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
