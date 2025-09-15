/*
  Copyright 2025 Peter Duanmu

  @file base/user/caster.hpp

  Group of dynamic_cast functions for easier use
*/

#ifndef KRONOS_BASE_USER_CASTER_HPP_
#define KRONOS_BASE_USER_CASTER_HPP_

#include "KRONOS/base/devices.hpp"

namespace kronos::base::user {
/*
  Casts AbstractDevice pointer to Button pointer

  @param device AbstractDevice pointer

  @return Button pointer
*/
Button* to_button(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Color pointer

  @param device AbstractDevice pointer

  @return Color pointer
*/
Color* to_color(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Controller pointer

  @param device AbstractDevice pointer

  @return Controller pointer
*/
Controller* to_controller(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Imu pointer

  @param device AbstractDevice pointer

  @return Imu pointer
*/
Imu* to_imu(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Motor pointer

  @param device AbstractDevice pointer

  @return Motor pointer
*/
Motor* to_motor(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to PID pointer

  @param device AbstractDevice pointer

  @return PID pointer
*/
PIDDevice* to_pid(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Piston pointer

  @param device AbstractDevice pointer

  @return Piston pointer
*/
Piston* to_piston(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Proximity pointer

  @param device AbstractDevice pointer

  @return Proximity pointer
*/
Proximity* to_proximity(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Rotation pointer

  @param device AbstractDevice pointer

  @return Rotation pointer
*/
Rotation* to_rotation(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to Vision pointer

  @param device AbstractDevice pointer

  @return Vision pointer
*/
Vision* to_vision(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to GPS pointer

  @param device AbstractDevice pointer

  @return GPS pointer
*/
GPS* to_gps(AbstractDevice* device);

/*
  Casts AbstractDevice pointer to LineTracker pointer

  @param device AbstractDevice pointer

  @return LineTracker pointer
*/
LineTracker* to_linetracker(AbstractDevice* device);
}  // namespace kronos::base::user

#endif  // KRONOS_BASE_USER_CASTER_HPP_
