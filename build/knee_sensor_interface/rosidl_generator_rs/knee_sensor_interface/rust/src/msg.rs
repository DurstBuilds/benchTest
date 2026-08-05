#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to knee_sensor_interface__msg__KneeSensor
/// Latching hall state (PIC HS-3511-02-0300)

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct KneeSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// true when latch is ON (open-drain OUT asserted, after active_low inversion)
    pub active: bool,

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
        active: msg.active,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      active: msg.active,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      active: msg.active,
    }
  }
}


