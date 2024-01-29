/*
  Copyright 2024 Peter Duanmu

  @file assets/devicestructs.hpp

  Device parameters
*/

#ifndef KRONOS_ASSETS_DEVICESTRUCTS_HPP_
#define KRONOS_ASSETS_DEVICESTRUCTS_HPP_

#include "assets/statics.hpp"

#include "base/extenders/pid.hpp"

#include "pros/apix.h"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

namespace KRONOS {
using KExtender::pid_exit_conditions;
using KExtender::pid_consts;
using KExtender::consistency_consts;

enum device_types {
  K_BUTTON = pros::c::E_DEVICE_ADI,
  K_COLOR = pros::c::E_DEVICE_OPTICAL,
  K_CONTROLLER = pros::c::E_DEVICE_RADIO,
  K_IMU = pros::c::E_DEVICE_IMU,
  K_MOTOR = pros::c::E_DEVICE_MOTOR,
  K_PID = 1 << 10,
  K_PISTON = pros::c::E_DEVICE_ADI,
  K_PROXIMITY = pros::c::E_DEVICE_DISTANCE,
  K_ROTATION = pros::c::E_DEVICE_ROTATION,
  K_VISION = pros::c::E_DEVICE_VISION
};

enum device_face {
  K_NORTH,
  K_NORTHEAST,
  K_EAST,
  K_SOUTHEAST,
  K_SOUTH,
  K_SOUTHWEST,
  K_WEST,
  K_NORTHWEST,
  K_NA
};

struct abstract_device_struct {
  int8_t port = 1;
  device_face face = K_NA;
  bool reverse = false;
};

struct controller_struct {
  pros::controller_id_e_t id = pros::E_CONTROLLER_MASTER;
};

struct motor_struct {
  int8_t port = 1;
  bool reverse = false;
  pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEAR_GREEN;
  pros::motor_encoder_units_e_t encoder = pros::E_MOTOR_ENCODER_DEGREES;
  pros::motor_brake_mode_e_t brakemode = pros::E_MOTOR_BRAKE_COAST;
  device_face face = K_NA;

  pid_exit_conditions pidexit = pid_exit_conditions::P_ERROR;
  pid_consts pidmods = pid_consts();
  consistency_consts consistencymods = consistency_consts();
};
}  // namespace KRONOS

#endif  // KRONOS_ASSETS_DEVICESTRUCTS_HPP_
