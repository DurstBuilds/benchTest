#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "knee_sensor_interface__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__knee_sensor_interface__msg__KneeSensor() -> *const std::ffi::c_void;
}

#[link(name = "knee_sensor_interface__rosidl_generator_c")]
extern "C" {
    fn knee_sensor_interface__msg__KneeSensor__init(msg: *mut KneeSensor) -> bool;
    fn knee_sensor_interface__msg__KneeSensor__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<KneeSensor>, size: usize) -> bool;
    fn knee_sensor_interface__msg__KneeSensor__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<KneeSensor>);
    fn knee_sensor_interface__msg__KneeSensor__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<KneeSensor>, out_seq: *mut rosidl_runtime_rs::Sequence<KneeSensor>) -> bool;
}

// Corresponds to knee_sensor_interface__msg__KneeSensor
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Knee sensor reading

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct KneeSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// Joint angle in radians
    pub angle: f64,

    /// Angular velocity in rad/s
    pub angular_velocity: f64,

    /// Optional force/torque or strain channels
    pub channels: rosidl_runtime_rs::Sequence<f64>,

}



impl Default for KneeSensor {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !knee_sensor_interface__msg__KneeSensor__init(&mut msg as *mut _) {
        panic!("Call to knee_sensor_interface__msg__KneeSensor__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for KneeSensor {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { knee_sensor_interface__msg__KneeSensor__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { knee_sensor_interface__msg__KneeSensor__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { knee_sensor_interface__msg__KneeSensor__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for KneeSensor {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for KneeSensor where Self: Sized {
  const TYPE_NAME: &'static str = "knee_sensor_interface/msg/KneeSensor";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__knee_sensor_interface__msg__KneeSensor() }
  }
}


