/*
  @file assets/devicestructs.hpp

  Device parameters
*/

#ifndef _DEVICESTRUCTS_HPP_
#define _DEVICESTRUCTS_HPP_

#include "assets/util.hpp"

#include "base/extenders/pid.hpp"

#include "pros/apix.h"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

namespace KRONOS {
  enum device_types {
    K_BUTTON = pros::c::E_DEVICE_ADI,
    K_COLOR = pros::c::E_DEVICE_OPTICAL,
    K_CONTROLLER = pros::c::E_DEVICE_RADIO,
    K_MOTOR = pros::c::E_DEVICE_MOTOR,
    K_PISTON = pros::c::E_DEVICE_ADI,
    K_PROXIMITY = pros::c::E_DEVICE_DISTANCE,
    K_VISION = pros::c::E_DEVICE_VISION
  };

  enum device_face {
    K_NONE,
    K_NORTH,
    K_NORTHEAST,
    K_EAST,
    K_SOUTHEAST,
    K_SOUTH,
    K_SOUTHWEST,
    K_WEST,
    K_NORTHWEST
  };

  struct abstract_device_struct {
    uint8_t port = 1;
    device_face face = K_NORTH;
  };

  struct controller_struct {
    pros::controller_id_e_t id = pros::E_CONTROLLER_MASTER;
  };

  struct motor_struct {
    uint8_t port = 1;
    pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEAR_GREEN;
    bool reverse = false;
    pros::motor_encoder_units_e_t encoder = pros::E_MOTOR_ENCODER_DEGREES;
    pros::motor_brake_mode_e_t brakemode = pros::E_MOTOR_BRAKE_COAST;
    device_face face = K_NORTH;

    KPID::pid_exit_conditions pidexit = KPID::pid_exit_conditions::P_ERROR;
    KPID::pid_consts pidmods = KPID::pid_consts();
  };
}

#endif
