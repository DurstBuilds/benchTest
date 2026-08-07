// experiment_manager_node: automated latch-angle sweep.
//
// Operator aligns shaft so arm 1 is at the reference pose, then calls
// ~/start_experiment. The node zeros the encoder, commands RPM through the
// schedule (6, 10..80 forward, settle at 0, then negative), and for each speed
// records Hall switch angles for both arms. After discarding the first sample
// per arm (speed-change transient), averages 10 samples and publishes means.
//
// Topics:
//   pub  motor_rpm, arm1_mean_angle, arm2_mean_angle
//   sub  encoder_angle, knee_sensor
// Services:
//   ~/start_experiment  — begin sweep (zeros encoder first)
// Client:
//   encoder_interface_node/zero

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "knee_sensor_interface/msg/knee_sensor.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_srvs/srv/trigger.hpp"

namespace
{
constexpr std::size_t kSamplesPerArm = 10;
constexpr double kRpmRepublishHz = 20.0;
constexpr double kSettleZeroSec = 2.0;
constexpr const char * kZeroService = "encoder_interface_node/zero";

/** Build forward then reverse magnitude list: 6, 10, 20, …, 80. */
std::vector<double> buildSpeedMagnitudes()
{
  std::vector<double> speeds;
  speeds.push_back(6.0);
  for (double rpm = 10.0; rpm <= 80.0 + 1e-9; rpm += 10.0) {
    speeds.push_back(rpm);
  }
  return speeds;
}

double wrap0To2Pi(double rad)
{
  double r = std::fmod(rad, 2.0 * M_PI);
  if (r < 0.0) {
    r += 2.0 * M_PI;
  }
  return r;
}

double circularDistance(double a_rad, double b_rad)
{
  const double d = a_rad - b_rad;
  return std::abs(std::atan2(std::sin(d), std::cos(d)));
}

/** Circular mean of angles near a reference (result ≈ reference + small error). */
double circularMeanAbout(const std::vector<double> & samples, double center_rad)
{
  double sum_sin = 0.0;
  double sum_cos = 0.0;
  for (const double theta : samples) {
    const double d = theta - center_rad;
    sum_sin += std::sin(d);
    sum_cos += std::cos(d);
  }
  const double n = static_cast<double>(samples.size());
  return center_rad + std::atan2(sum_sin / n, sum_cos / n);
}

enum class RunPhase
{
  Idle,
  Zeroing,
  Collecting,
  SettlingZero,
  Finished,
};
}  // namespace

class ExperimentManagerNode : public rclcpp::Node
{
public:
  ExperimentManagerNode()
  : Node("experiment_manager_node")
  {
    const auto rpm_topic = declare_parameter<std::string>("rpm_topic", "motor_rpm");
    const auto angle_topic = declare_parameter<std::string>("angle_topic", "encoder_angle");
    const auto sensor_topic = declare_parameter<std::string>("sensor_topic", "knee_sensor");
    const auto arm1_topic =
      declare_parameter<std::string>("arm1_mean_topic", "arm1_mean_angle");
    const auto arm2_topic =
      declare_parameter<std::string>("arm2_mean_topic", "arm2_mean_angle");
    settle_zero_sec_ = declare_parameter<double>("settle_zero_sec", kSettleZeroSec);
    samples_per_arm_ = static_cast<std::size_t>(
      declare_parameter<int>("samples_per_arm", static_cast<int>(kSamplesPerArm)));

    rpm_pub_ = create_publisher<std_msgs::msg::Float64>(rpm_topic, 10);
    arm1_mean_pub_ = create_publisher<std_msgs::msg::Float64>(arm1_topic, 10);
    arm2_mean_pub_ = create_publisher<std_msgs::msg::Float64>(arm2_topic, 10);

    angle_sub_ = create_subscription<std_msgs::msg::Float64>(
      angle_topic, rclcpp::SensorDataQoS(),
      [this](const std_msgs::msg::Float64::SharedPtr msg) { latest_angle_rad_ = msg->data; });

    sensor_sub_ = create_subscription<knee_sensor_interface::msg::KneeSensor>(
      sensor_topic, rclcpp::QoS(10).reliable(),
      [this](const knee_sensor_interface::msg::KneeSensor::SharedPtr msg) { onSensor(msg); });

    zero_client_ = create_client<std_srvs::srv::Trigger>(kZeroService);

    start_srv_ = create_service<std_srvs::srv::Trigger>(
      "~/start_experiment",
      [this](
        const std::shared_ptr<std_srvs::srv::Trigger::Request> /*request*/,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        onStartExperiment(response);
      });

    const auto period = std::chrono::duration<double>(1.0 / kRpmRepublishHz);
    tick_timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      [this]() { onTick(); });

    magnitudes_ = buildSpeedMagnitudes();

    RCLCPP_INFO(
      get_logger(),
      "Experiment manager idle. Align arm 1, then call ~/start_experiment. "
      "Samples/arm=%zu settle=%.1fs",
      samples_per_arm_, settle_zero_sec_);
  }

  ~ExperimentManagerNode() override
  {
    publishRpm(0.0);
  }

private:
  void onStartExperiment(const std::shared_ptr<std_srvs::srv::Trigger::Response> response)
  {
    if (phase_ != RunPhase::Idle && phase_ != RunPhase::Finished) {
      response->success = false;
      response->message = "Experiment already running";
      return;
    }

    if (!zero_client_->service_is_ready()) {
      response->success = false;
      response->message = std::string("Zero service not ready: ") + kZeroService;
      return;
    }

    phase_ = RunPhase::Zeroing;
    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    zero_client_->async_send_request(
      request,
      [this](rclcpp::Client<std_srvs::srv::Trigger>::SharedFuture future) {
        onZeroResponse(future);
      });

    response->success = true;
    response->message = "Zeroing encoder, then starting RPM sweep";
    RCLCPP_INFO(get_logger(), "%s", response->message.c_str());
  }

  void onZeroResponse(const rclcpp::Client<std_srvs::srv::Trigger>::SharedFuture future)
  {
    const auto result = future.get();
    if (!result->success) {
      RCLCPP_ERROR(get_logger(), "Encoder zero failed: %s", result->message.c_str());
      phase_ = RunPhase::Idle;
      publishRpm(0.0);
      return;
    }

    RCLCPP_INFO(get_logger(), "Encoder zeroed: %s", result->message.c_str());
    direction_sign_ = 1.0;
    speed_index_ = 0;
    beginSpeedStep();
  }

  void beginSpeedStep()
  {
    if (speed_index_ >= magnitudes_.size()) {
      if (direction_sign_ > 0.0) {
        // Finished forward sweep: stop, settle, then reverse.
        commanded_rpm_ = 0.0;
        publishRpm(0.0);
        phase_ = RunPhase::SettlingZero;
        settle_deadline_ = now() + rclcpp::Duration::from_seconds(settle_zero_sec_);
        RCLCPP_INFO(
          get_logger(), "Forward sweep complete; settling at 0 RPM for %.1fs", settle_zero_sec_);
        return;
      }

      commanded_rpm_ = 0.0;
      publishRpm(0.0);
      phase_ = RunPhase::Finished;
      RCLCPP_INFO(get_logger(), "Experiment complete (forward and reverse). Motor stopped.");
      return;
    }

    commanded_rpm_ = direction_sign_ * magnitudes_[speed_index_];
    arm1_samples_.clear();
    arm2_samples_.clear();
    discard_arm1_ = true;
    discard_arm2_ = true;
    prev_active_.reset();
    phase_ = RunPhase::Collecting;
    publishRpm(commanded_rpm_);

    RCLCPP_INFO(
      get_logger(),
      "Collecting at %.1f RPM (need %zu samples/arm after discard)",
      commanded_rpm_, samples_per_arm_);
  }

  void onTick()
  {
    if (phase_ == RunPhase::Collecting || phase_ == RunPhase::SettlingZero) {
      publishRpm(commanded_rpm_);
    }

    if (phase_ == RunPhase::SettlingZero && now() >= settle_deadline_) {
      direction_sign_ = -1.0;
      speed_index_ = 0;
      RCLCPP_INFO(get_logger(), "Starting reverse sweep");
      beginSpeedStep();
    }
  }

  void onSensor(const knee_sensor_interface::msg::KneeSensor::SharedPtr msg)
  {
    if (phase_ != RunPhase::Collecting) {
      return;
    }
    if (!latest_angle_rad_.has_value()) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000,
        "Ignoring latch edge: no encoder_angle yet");
      return;
    }

    if (!prev_active_.has_value()) {
      prev_active_ = msg->active;
      return;
    }
    if (msg->active == *prev_active_) {
      return;
    }
    prev_active_ = msg->active;

    const double angle = wrap0To2Pi(*latest_angle_rad_);
    const bool is_arm1 = circularDistance(angle, 0.0) < circularDistance(angle, M_PI);
    recordSample(is_arm1, angle);

    if (arm1_samples_.size() >= samples_per_arm_ &&
      arm2_samples_.size() >= samples_per_arm_)
    {
      publishMeansAndAdvance();
    }
  }

  void recordSample(bool is_arm1, double angle_rad)
  {
    bool & discard = is_arm1 ? discard_arm1_ : discard_arm2_;
    auto & samples = is_arm1 ? arm1_samples_ : arm2_samples_;
    const char * arm_name = is_arm1 ? "arm1" : "arm2";

    if (discard) {
      discard = false;
      RCLCPP_INFO(
        get_logger(),
        "Discarding first %s sample at %.1f RPM: %.6f rad",
        arm_name, commanded_rpm_, angle_rad);
      return;
    }

    if (samples.size() >= samples_per_arm_) {
      return;
    }

    samples.push_back(angle_rad);
    RCLCPP_INFO(
      get_logger(),
      "Kept %s sample %zu/%zu at %.1f RPM: %.6f rad",
      arm_name, samples.size(), samples_per_arm_, commanded_rpm_, angle_rad);
  }

  void publishMeansAndAdvance()
  {
    const double mean1 = circularMeanAbout(arm1_samples_, 0.0);
    const double mean2 = circularMeanAbout(arm2_samples_, M_PI);

    std_msgs::msg::Float64 msg;
    msg.data = mean1;
    arm1_mean_pub_->publish(msg);
    msg.data = mean2;
    arm2_mean_pub_->publish(msg);

    RCLCPP_INFO(
      get_logger(),
      "Means at %.1f RPM: arm1=%.6f rad (%.2f deg), arm2=%.6f rad (%.2f deg)",
      commanded_rpm_, mean1, mean1 * 180.0 / M_PI, mean2, mean2 * 180.0 / M_PI);

    ++speed_index_;
    beginSpeedStep();
  }

  void publishRpm(double rpm)
  {
    commanded_rpm_ = rpm;
    std_msgs::msg::Float64 msg;
    msg.data = rpm;
    rpm_pub_->publish(msg);
  }

  double settle_zero_sec_ {kSettleZeroSec};
  std::size_t samples_per_arm_ {kSamplesPerArm};

  RunPhase phase_ {RunPhase::Idle};
  double direction_sign_ {1.0};
  std::size_t speed_index_ {0};
  std::vector<double> magnitudes_;
  double commanded_rpm_ {0.0};
  rclcpp::Time settle_deadline_ {0, 0, RCL_ROS_TIME};

  std::optional<double> latest_angle_rad_;
  std::optional<bool> prev_active_;
  std::vector<double> arm1_samples_;
  std::vector<double> arm2_samples_;
  bool discard_arm1_ {true};
  bool discard_arm2_ {true};

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr rpm_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm1_mean_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr arm2_mean_pub_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr angle_sub_;
  rclcpp::Subscription<knee_sensor_interface::msg::KneeSensor>::SharedPtr sensor_sub_;
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr zero_client_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr start_srv_;
  rclcpp::TimerBase::SharedPtr tick_timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<ExperimentManagerNode>();
    rclcpp::spin(node);
  } catch (const std::exception & ex) {
    fprintf(stderr, "experiment_manager_node failed to start: %s\n", ex.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
