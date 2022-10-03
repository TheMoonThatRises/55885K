/*
  @file assets/devicestructs.hpp

  Device parameters
*/

#ifndef _DEVICESTRUCTS_HPP_
#define _DEVICESTRUCTS_HPP_

#include "assets/util.hpp"

#include "base/extenders/pid.hpp"

#include "pros/misc.hpp"
#include "pros/motors.hpp"

namespace KRONOS {
  enum device_types {
    K_BUTTON,
    K_COLOR,
    K_CONTROLLER,
    K_MOTOR,
    K_PISTON,
    K_PROXIMITY,
    K_VISION
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


  struct AbstractDeviceStruct {
    uint8_t port = 1;
    device_face face = K_NORTH;
  };

  struct ControllerStruct {
    pros::controller_id_e_t id = pros::E_CONTROLLER_MASTER;
  };

  struct MotorStruct {
    uint8_t port = 1;
    pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEAR_GREEN;
    bool reverse = false;
    pros::motor_encoder_units_e_t encoder = pros::E_MOTOR_ENCODER_DEGREES;
    pros::motor_brake_mode_e_t brakemode = pros::E_MOTOR_BRAKE_COAST;
    bool lock = true;
    int lockdelay = MSDELAY;
    device_face face = K_NORTH;

    KPID::pid_exit_conditions pidexit = KPID::pid_exit_conditions::error;
  };
}

#endif
