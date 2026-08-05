#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to knee_sensor_interface__msg__KneeSensor
/// Knee sensor reading

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct KneeSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// Joint angle in radians
    pub angle: f64,

    /// Angular velocity in rad/s
    pub angular_velocity: f64,

    /// Optional force/torque or strain channels
    pub channels: Vec<f64>,

}



impl Default for KneeSensor {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::KneeSensor::default())
  }
}

impl rosidl_runtime_rs::Message for KneeSensor {
  type RmwMsg = super::msg::rmw::KneeSensor;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        angle: msg.angle,
        angular_velocity: msg.angular_velocity,
        channels: msg.channels.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      angle: msg.angle,
      angular_velocity: msg.angular_velocity,
        channels: msg.channels.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      angle: msg.angle,
      angular_velocity: msg.angular_velocity,
      channels: msg.channels
          .into_iter()
          .collect(),
    }
  }
}


