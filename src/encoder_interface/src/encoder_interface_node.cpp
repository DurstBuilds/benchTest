// encoder_interface_node: read AS5047D shaft angle over Linux spidev and
// publish radians on encoder_angle (std_msgs/Float64).
//
// Hardware: AS5047D-TS_EK_AB eval board, 4-wire SPI (CSn, CLK, MOSI, MISO).
// Board must be powered; magnet centered with 0.5–3 mm airgap. Process needs
// read/write access to /dev/spidev* (udev rule or membership in the spi group).
//
// Parameters (TWEAK via launch or ros2 param):
//   spi_device         — spidev path (default /dev/spidev0.0)
//   spi_speed_hz       — SPI clock Hz (default 1000000)
//   publish_rate_hz    — read/publish rate (default 100.0)
//   angle_topic        — publication name (default encoder_angle)
//   angle_offset_rad   — added after raw→rad conversion (default 0.0)

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <unistd.h>

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

namespace
{
constexpr uint16_t kRegAngleCom = 0x3FFF;
constexpr uint16_t kCmdRead = 0x4000;
constexpr uint16_t kNopFrame = 0x0000;

constexpr uint16_t kParityMask = 0x8000;
constexpr uint16_t kErrorFlagMask = 0x4000;
constexpr uint16_t kDataMask = 0x3FFF;

constexpr uint32_t kAngleCounts = 16384;
constexpr uint8_t kSpiMode = SPI_MODE_1;
constexpr uint8_t kSpiBitsPerWord = 8;

// Even parity over all 16 bits (including the parity bit itself).
bool hasEvenParity(uint16_t frame)
{
  return (__builtin_popcount(frame) % 2) == 0;
}

// Build a 16-bit AS5047D command with even parity in bit 15.
uint16_t makeCommand(uint16_t rw_and_address)
{
  const uint16_t lower = static_cast<uint16_t>(rw_and_address & 0x7FFF);
  return hasEvenParity(lower) ? lower : static_cast<uint16_t>(lower | kParityMask);
}
}  // namespace

class EncoderInterfaceNode : public rclcpp::Node
{
public:
  EncoderInterfaceNode()
  : Node("encoder_interface_node")
  {
    // TWEAK: spidev path, clock, publish rate, topic, and zero offset
    spi_device_ = declare_parameter<std::string>("spi_device", "/dev/spidev0.0");
    spi_speed_hz_ = declare_parameter<int>("spi_speed_hz", 1000000);
    const double publish_rate_hz = declare_parameter<double>("publish_rate_hz", 100.0);
    const auto angle_topic = declare_parameter<std::string>("angle_topic", "encoder_angle");
    angle_offset_rad_ = declare_parameter<double>("angle_offset_rad", 0.0);

    if (spi_speed_hz_ <= 0) {
      throw std::runtime_error("spi_speed_hz must be positive");
    }
    if (publish_rate_hz <= 0.0) {
      throw std::runtime_error("publish_rate_hz must be positive");
    }

    if (!openSpi()) {
      throw std::runtime_error(
        "Failed to open SPI device '" + spi_device_ + "': " + std::strerror(errno));
    }

    // Prime the AS5047D pipeline so the first timer tick returns a valid ANGLECOM sample.
    uint16_t unused = 0;
    if (!transferFrame(makeCommand(kCmdRead | kRegAngleCom), unused) ||
      !transferFrame(kNopFrame, unused))
    {
      closeSpi();
      throw std::runtime_error("Failed to prime AS5047D ANGLECOM read over SPI");
    }

    publisher_ = create_publisher<std_msgs::msg::Float64>(angle_topic, rclcpp::SensorDataQoS());

    const auto period = std::chrono::duration<double>(1.0 / publish_rate_hz);
    timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      [this]() { onTimer(); });

    RCLCPP_INFO(
      get_logger(),
      "Publishing AS5047D angle on '%s' at %.1f Hz via %s (%d Hz)",
      angle_topic.c_str(), publish_rate_hz, spi_device_.c_str(), spi_speed_hz_);
  }

  ~EncoderInterfaceNode() override
  {
    closeSpi();
  }

private:
  // Open spidev and set Mode 1, 8-bit words, and configured clock.
  bool openSpi()
  {
    spi_fd_ = ::open(spi_device_.c_str(), O_RDWR);
    if (spi_fd_ < 0) {
      return false;
    }

    uint8_t mode = kSpiMode;
    if (ioctl(spi_fd_, SPI_IOC_WR_MODE, &mode) < 0) {
      closeSpi();
      return false;
    }
    uint8_t bits = kSpiBitsPerWord;
    if (ioctl(spi_fd_, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
      closeSpi();
      return false;
    }

    uint32_t speed = static_cast<uint32_t>(spi_speed_hz_);
    if (ioctl(spi_fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
      closeSpi();
      return false;
    }

    return true;
  }

  void closeSpi()
  {
    if (spi_fd_ >= 0) {
      ::close(spi_fd_);
      spi_fd_ = -1;
    }
  }

  // One 16-bit AS5047D frame; CS rises after the transfer (separate SPI message).
  bool transferFrame(uint16_t tx_frame, uint16_t & rx_frame)
  {
    uint8_t tx[2] = {
      static_cast<uint8_t>((tx_frame >> 8) & 0xFF),
      static_cast<uint8_t>(tx_frame & 0xFF),
    };
    uint8_t rx[2] = {0, 0};

    spi_ioc_transfer xfer {};
    xfer.tx_buf = reinterpret_cast<__u64>(tx);
    xfer.rx_buf = reinterpret_cast<__u64>(rx);
    xfer.len = sizeof(tx);
    xfer.speed_hz = static_cast<__u32>(spi_speed_hz_);
    xfer.bits_per_word = kSpiBitsPerWord;

    if (ioctl(spi_fd_, SPI_IOC_MESSAGE(1), &xfer) < 0) {
      return false;
    }

    rx_frame = static_cast<uint16_t>((rx[0] << 8) | rx[1]);
    return true;
  }

  // Read ANGLECOM: send READ, then NOP to clock out the prior response.
  bool readAngleRaw(uint16_t & angle_raw)
  {
    uint16_t discarded = 0;
    const uint16_t cmd = makeCommand(kCmdRead | kRegAngleCom);
    if (!transferFrame(cmd, discarded)) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "SPI transfer failed (command): %s", std::strerror(errno));
      return false;
    }

    uint16_t response = 0;
    if (!transferFrame(kNopFrame, response)) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "SPI transfer failed (NOP): %s", std::strerror(errno));
      return false;
    }

    if (!hasEvenParity(response)) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "AS5047D response parity error (0x%04x)", response);
      return false;
    }
    if ((response & kErrorFlagMask) != 0) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "AS5047D error flag set (0x%04x)", response);
      return false;
    }

    angle_raw = static_cast<uint16_t>(response & kDataMask);
    return true;
  }

  void onTimer()
  {
    uint16_t angle_raw = 0;
    if (!readAngleRaw(angle_raw)) {
      return;
    }

    std_msgs::msg::Float64 msg;
    msg.data =
      (static_cast<double>(angle_raw) * (2.0 * M_PI / static_cast<double>(kAngleCounts))) +
      angle_offset_rad_;
    publisher_->publish(msg);

    RCLCPP_DEBUG(get_logger(), "Angle: raw=%u rad=%.6f", angle_raw, msg.data);
  }

  std::string spi_device_;
  int spi_speed_hz_ {1000000};
  double angle_offset_rad_ {0.0};
  int spi_fd_ {-1};
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<EncoderInterfaceNode>();
    rclcpp::spin(node);
  } catch (const std::exception & ex) {
    fprintf(stderr, "encoder_interface_node failed to start: %s\n", ex.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
