/*
  @file assets/devicestructs.hpp

  Device parameters
*/

#ifndef _DEVICESTRUCTS_HPP_
#define _DEVICESTRUCTS_HPP_

#include "assets/util.hpp"

#include "pros/misc.hpp"
#include "pros/motors.hpp"

namespace KRONOS {
  struct AbstractDeviceStruct {
    uint8_t port = 1;
  };

  struct ControllerStruct {
    pros::controller_id_e_t id = pros::E_CONTROLLER_MASTER;
  };

  struct MotorStruct {
    uint8_t port = 1;
    pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18;
    bool reverse = false;
    pros::motor_encoder_units_e_t encoder = pros::E_MOTOR_ENCODER_DEGREES;
    pros::motor_brake_mode_e_t brakemode = pros::E_MOTOR_BRAKE_COAST;
    bool lock = true;
    int lockdelay = MSDELAY;
  };
}

#endif