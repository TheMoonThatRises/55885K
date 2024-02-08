/*
  Copyright 2024 Peter Duanmu

  @file base/user/caster.hpp

  Group of dynamic_cast functions for easier use
*/

#ifndef KRONOS_BASE_USER_CASTER_HPP_
#define KRONOS_BASE_USER_CASTER_HPP_

#include "assets/asserts.hpp"

#include "base/devices.hpp"

namespace KRONOS {
/*
  Casts AbstractDevice pointer to Button pointer

  @param device AbstractDevice pointer

  @return Button pointer
*/
Button* to_button(AbstractDevice* device) {
  assert_same_device(device, K_BUTTON);

  return dynamic_cast<Button*>(device);
}

/*
  Casts AbstractDevice pointer to Color pointer

  @param device AbstractDevice pointer

  @return Color pointer
*/
Color* to_color(AbstractDevice* device) {
  assert_same_device(device, K_COLOR);

  return dynamic_cast<Color*>(device);
}

/*
  Casts AbstractDevice pointer to Controller pointer

  @param device AbstractDevice pointer

  @return Controller pointer
*/
Controller* to_controller(AbstractDevice* device) {
  assert_same_device(device, K_CONTROLLER);

  return dynamic_cast<Controller*>(device);
}

/*
  Casts AbstractDevice pointer to Motor pointer

  @param device AbstractDevice pointer

  @return Motor pointer
*/
Motor* to_motor(AbstractDevice* device) {
  assert_same_device(device, K_MOTOR);

  return dynamic_cast<Motor*>(device);
}

/*
  Casts AbstractDevice pointer to PID pointer

  @param device AbstractDevice pointer

  @return PID pointer
*/
PIDDevice* to_pid(AbstractDevice* device) {
  assert_same_device(device, K_PID);

  return dynamic_cast<PIDDevice*>(device);
}

/*
  Casts AbstractDevice pointer to Piston pointer

  @param device AbstractDevice pointer

  @return Piston pointer
*/
Piston* to_piston(AbstractDevice* device) {
  assert_same_device(device, K_PISTON);

  return dynamic_cast<Piston*>(device);
}

/*
  Casts AbstractDevice pointer to Proximity pointer

  @param device AbstractDevice pointer

  @return Proximity pointer
*/
Proximity* to_proximity(AbstractDevice* device) {
  assert_same_device(device, K_PROXIMITY);

  return dynamic_cast<Proximity*>(device);
}

/*
  Casts AbstractDevice pointer to Rotation pointer

  @param device AbstractDevice pointer

  @return Rotation pointer
*/
Rotation* to_rotation(AbstractDevice* device) {
  assert_same_device(device, K_ROTATION);

  return dynamic_cast<Rotation*>(device);
}

/*
  Casts AbstractDevice pointer to Vision pointer

  @param device AbstractDevice pointer

  @return Vision pointer
*/
Vision* to_vision(AbstractDevice* device) {
  assert_same_device(device, K_VISION);

  return dynamic_cast<Vision*>(device);
}
}  // namespace KRONOS

#endif  // KRONOS_BASE_USER_CASTER_HPP_
