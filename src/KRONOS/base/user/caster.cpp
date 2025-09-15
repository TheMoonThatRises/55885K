/*
  Copyright 2025 Peter Duanmu

  @file base/user/caster.cpp

  Group of dynamic_cast functions for easier use
*/

#include "KRONOS/base/user/caster.hpp"

#include "KRONOS/assets/asserts.hpp"

namespace kronos::base::user {
using assets::assert_same_device;
using assets::device_types;

Button* to_button(AbstractDevice* device) {
  assert_same_device(device, device_types::K_BUTTON);

  return dynamic_cast<Button*>(device);
}

Color* to_color(AbstractDevice* device) {
  assert_same_device(device, device_types::K_COLOR);

  return dynamic_cast<Color*>(device);
}

Controller* to_controller(AbstractDevice* device) {
  assert_same_device(device, device_types::K_CONTROLLER);

  return dynamic_cast<Controller*>(device);
}

Imu* to_imu(AbstractDevice* device) {
  assert_same_device(device, device_types::K_IMU);

  return dynamic_cast<Imu*>(device);
}

Motor* to_motor(AbstractDevice* device) {
  assert_same_device(device, device_types::K_MOTOR);

  return dynamic_cast<Motor*>(device);
}

PIDDevice* to_pid(AbstractDevice* device) {
  assert_same_device(device, device_types::K_PID);

  return dynamic_cast<PIDDevice*>(device);
}

Piston* to_piston(AbstractDevice* device) {
  assert_same_device(device, device_types::K_PISTON);

  return dynamic_cast<Piston*>(device);
}

Proximity* to_proximity(AbstractDevice* device) {
  assert_same_device(device, device_types::K_PROXIMITY);

  return dynamic_cast<Proximity*>(device);
}

Rotation* to_rotation(AbstractDevice* device) {
  assert_same_device(device, device_types::K_ROTATION);

  return dynamic_cast<Rotation*>(device);
}

Vision* to_vision(AbstractDevice* device) {
  assert_same_device(device, device_types::K_VISION);

  return dynamic_cast<Vision*>(device);
}

GPS* to_gps(AbstractDevice* device) {
  assert_same_device(device, device_types::K_GPS);

  return dynamic_cast<GPS*>(device);
}

LineTracker* to_linetracker(AbstractDevice* device) {
  assert_same_device(device, device_types::K_LINE_TRACKER);

  return dynamic_cast<LineTracker*>(device);
}
}  // namespace kronos::base::user
