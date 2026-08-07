// keyboard_rpm_node: prompt for RPM on the controlling terminal and publish
// std_msgs/Float64 to motor_rpm for odrive_interface_node.
//
// Reads /dev/tty (not stdin) so input still works under ros2 launch, which
// does not forward the launch terminal's stdin to child nodes.
//
// Parameters (TWEAK via launch or ros2 param):
//   rpm_topic  — publish topic (default motor_rpm)
//
// Type a number and press Enter. Empty / non-numeric lines are ignored.
// Ctrl+D or "q" exits and shuts the node down.

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

namespace
{
constexpr const char * kPrompt = "Enter motor RPM: ";
constexpr const char * kTtyPath = "/dev/tty";
}  // namespace

class KeyboardRpmNode : public rclcpp::Node
{
public:
  KeyboardRpmNode()
  : Node("keyboard_rpm_node")
  {
    // TWEAK: topic consumed by odrive_interface_node
    rpm_topic_ = declare_parameter<std::string>("rpm_topic", "motor_rpm");
    publisher_ = create_publisher<std_msgs::msg::Float64>(rpm_topic_, 10);

    RCLCPP_INFO(
      get_logger(),
      "Publishing keyboard RPM commands on '%s' (read from %s)",
      rpm_topic_.c_str(), kTtyPath);
  }

  /**
   * Block on the controlling TTY, prompt for RPM, and publish each valid value.
   * Call from the main thread; spin ROS separately.
   */
  void runPromptLoop()
  {
    // Launch redirects node stdin; /dev/tty is the real keyboard terminal.
    std::ifstream tty(kTtyPath);
    if (!tty.is_open()) {
      RCLCPP_ERROR(get_logger(), "Failed to open %s for keyboard input", kTtyPath);
      rclcpp::shutdown();
      return;
    }

    std::string line;
    while (rclcpp::ok()) {
      std::cout << kPrompt << std::flush;
      if (!std::getline(tty, line)) {
        break;
      }

      if (line == "q" || line == "Q") {
        break;
      }
      if (line.empty()) {
        continue;
      }

      try {
        const double rpm = std::stod(line);
        std_msgs::msg::Float64 msg;
        msg.data = rpm;
        publisher_->publish(msg);
        RCLCPP_INFO(get_logger(), "Published motor_rpm: %.3f", rpm);
      } catch (const std::exception &) {
        std::cerr << "Invalid RPM '" << line << "'; enter a number or q to quit.\n";
      }
    }

    rclcpp::shutdown();
  }

private:
  std::string rpm_topic_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<KeyboardRpmNode>();

  std::thread spin_thread([node]() { rclcpp::spin(node); });
  node->runPromptLoop();

  if (spin_thread.joinable()) {
    spin_thread.join();
  }
  return 0;
}
