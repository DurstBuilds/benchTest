// odrive_interface_node: velocity-control an ODrive Micro over SocketCAN.
// Subscribes to motor_rpm (Float64, RPM) and sends CANSimple Set_Input_Vel
// (turns/s = rpm / 60). Assumes the ODrive is already calibrated.
//
// Parameters (TWEAK via launch or ros2 param):
//   can_interface  — SocketCAN device (default can0)
//   node_id        — ODrive CANSimple node ID (default 0; must match axis.config.can.node_id)
//   rpm_topic      — subscription name (default motor_rpm)
//
// Startup: bind SocketCAN, Set_Controller_Mode (velocity + passthrough),
// Set_Axis_State (CLOSED_LOOP_CONTROL). Does not wait on heartbeats.
// Shutdown: Set_Input_Vel(0) then axis IDLE.
// CANSimple arb ID = (node_id << 5) | cmd_id; payloads little-endian.

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

namespace
{
// CANSimple command IDs (ODrive protocol)
constexpr uint32_t kCmdSetAxisState = 0x07;
constexpr uint32_t kCmdSetControllerMode = 0x0b;
constexpr uint32_t kCmdSetInputVel = 0x0d;

constexpr uint32_t kAxisStateIdle = 1;
constexpr uint32_t kAxisStateClosedLoopControl = 8;

constexpr uint32_t kControlModeVelocity = 2;
constexpr uint32_t kInputModePassthrough = 1;
}  // namespace

class ODriveInterfaceNode : public rclcpp::Node
{
public:
  ODriveInterfaceNode()
  : Node("odrive_interface_node")
  {
    // TWEAK: SocketCAN iface, ODrive node ID, and command topic
    can_interface_ = declare_parameter<std::string>("can_interface", "can0");
    node_id_ = declare_parameter<int>("node_id", 0);
    const auto rpm_topic = declare_parameter<std::string>("rpm_topic", "motor_rpm");

    if (node_id_ < 0 || node_id_ > 0x3f) {
      throw std::runtime_error("node_id must be in range [0, 63]");
    }

    if (!openCanSocket()) {
      throw std::runtime_error(
        "Failed to open SocketCAN interface '" + can_interface_ + "': " + std::strerror(errno));
    }

    if (!configureVelocityControl()) {
      closeCanSocket();
      throw std::runtime_error("Failed to configure ODrive for velocity control over CAN");
    }

    subscription_ = create_subscription<std_msgs::msg::Float64>(
      rpm_topic, rclcpp::SensorDataQoS(),
      [this](const std_msgs::msg::Float64::SharedPtr msg) { onMotorRpm(msg); });

    RCLCPP_INFO(
      get_logger(),
      "Listening on '%s' for RPM commands; controlling ODrive node_id=%d on %s",
      rpm_topic.c_str(), node_id_, can_interface_.c_str());
  }

  // Safe stop: zero velocity then disarm to IDLE before closing the socket.
  ~ODriveInterfaceNode() override
  {
    if (can_socket_ >= 0) {
      sendInputVel(0.0f);
      sendAxisState(kAxisStateIdle);
      closeCanSocket();
    }
  }

private:
  // Bind a raw CAN socket to can_interface_. Fails if the iface is down/missing.
  bool openCanSocket()
  {
    can_socket_ = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_socket_ < 0) {
      return false;
    }

    ifreq ifr {};
    std::strncpy(ifr.ifr_name, can_interface_.c_str(), IFNAMSIZ - 1);
    if (ioctl(can_socket_, SIOCGIFINDEX, &ifr) < 0) {
      closeCanSocket();
      return false;
    }

    sockaddr_can addr {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_socket_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
      closeCanSocket();
      return false;
    }

    return true;
  }

  void closeCanSocket()
  {
    if (can_socket_ >= 0) {
      ::close(can_socket_);
      can_socket_ = -1;
    }
  }

  // CANSimple 11-bit ID: upper 6 bits = node_id, lower 5 bits = cmd_id.
  uint32_t arbitrationId(uint32_t cmd_id) const
  {
    return (static_cast<uint32_t>(node_id_) << 5) | cmd_id;
  }

  bool sendFrame(uint32_t cmd_id, const uint8_t * data, uint8_t len)
  {
    can_frame frame {};
    frame.can_id = arbitrationId(cmd_id);
    frame.can_dlc = len;
    if (len > 0 && data != nullptr) {
      std::memcpy(frame.data, data, len);
    }

    const auto nbytes = ::write(can_socket_, &frame, sizeof(frame));
    if (nbytes != static_cast<ssize_t>(sizeof(frame))) {
      RCLCPP_ERROR(
        get_logger(), "CAN write failed for cmd 0x%02x: %s", cmd_id, std::strerror(errno));
      return false;
    }
    return true;
  }

  bool sendAxisState(uint32_t state)
  {
    uint8_t payload[4];
    std::memcpy(payload, &state, sizeof(state));
    return sendFrame(kCmdSetAxisState, payload, sizeof(payload));
  }

  bool sendControllerMode(uint32_t control_mode, uint32_t input_mode)
  {
    uint8_t payload[8];
    std::memcpy(payload, &control_mode, sizeof(control_mode));
    std::memcpy(payload + 4, &input_mode, sizeof(input_mode));
    return sendFrame(kCmdSetControllerMode, payload, sizeof(payload));
  }

  // Set_Input_Vel: float32 turns/s + float32 torque feedforward (Nm).
  bool sendInputVel(float turns_per_sec, float torque_ff = 0.0f)
  {
    uint8_t payload[8];
    std::memcpy(payload, &turns_per_sec, sizeof(turns_per_sec));
    std::memcpy(payload + 4, &torque_ff, sizeof(torque_ff));
    return sendFrame(kCmdSetInputVel, payload, sizeof(payload));
  }

  // One-shot mode/state setup; ODrive must already be motor-calibrated.
  bool configureVelocityControl()
  {
    if (!sendControllerMode(kControlModeVelocity, kInputModePassthrough)) {
      return false;
    }
    if (!sendAxisState(kAxisStateClosedLoopControl)) {
      return false;
    }

    RCLCPP_INFO(get_logger(), "Configured ODrive for closed-loop velocity control");
    return true;
  }

  // Convert RPM → turns/s and stream Set_Input_Vel (also feeds ODrive watchdog).
  void onMotorRpm(const std_msgs::msg::Float64::SharedPtr msg)
  {
    const float turns_per_sec = static_cast<float>(msg->data / 60.0);
    if (!sendInputVel(turns_per_sec)) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "Failed to send Set_Input_Vel for %.3f RPM", msg->data);
      return;
    }

    RCLCPP_DEBUG(
      get_logger(), "Set velocity: %.3f RPM (%.4f turns/s)", msg->data, turns_per_sec);
  }

  std::string can_interface_;
  int node_id_ {0};
  int can_socket_ {-1};
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<ODriveInterfaceNode>();
    rclcpp::spin(node);
  } catch (const std::exception & ex) {
    fprintf(stderr, "odrive_interface_node failed to start: %s\n", ex.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
