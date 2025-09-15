/*
  Copyright 2025 Peter Duanmu

  @file base/managers/chassismanager.cpp

  Chassis manager for the KRONOS API
*/

#include <algorithm>
#include <vector>

#include "KRONOS/base/managers/chassismanager.hpp"

#include "KRONOS/assets/asserts.hpp"
#include "KRONOS/assets/logger.hpp"

namespace kronos::base::managers {
using assets::assert_not_nullptr;
using assets::KRONOS_JOYSTICK_MOTOR_RATIO;
using assets::pid_consts;
using assets::device_face;
using assets::pid_exit_conditions;;

void ChassisManager::set_motors(const std::vector<Motor*>& motors) {
  _chassisMotors = motors;
}

void ChassisManager::set_odoms(const std::vector<Rotation*>& odoms) {
  _odomSensors = odoms;
}

void ChassisManager::use_pid(const bool& use) {
  _use_pid = use;
}

ChassisManager::ChassisManager() : PID(pid_exit_conditions::P_NONE, {}, {}) {
  _chassisMotors.reserve(8);
  _odomSensors.reserve(8);
}

void ChassisManager::move_chassis(
  const double& straight,
  const double& strafe,
  const double& turn) const {
  for (Motor *motor : _chassisMotors) {
    assert_not_nullptr(motor, "KRONOS::Motor");

    const double mstraight =
      (
        motor->facing() >= device_face::K_NORTHEAST &&
        motor->facing() <= device_face::K_SOUTHEAST)
        ? -straight
        : straight;
    const double mstrafe =
      (
        motor->facing() >= device_face::K_SOUTHEAST &&
        motor->facing() <= device_face::K_SOUTHWEST)
        ? -strafe
        : strafe;

    const double target_velocity =
      (mstraight + mstrafe + turn) * KRONOS_JOYSTICK_MOTOR_RATIO;

    // motor->set_max_speed(target_velocity);

    if (_use_pid) {
      (void) motor->move_velocity_pid(target_velocity);
    } else {
      (void) motor->move_velocity(target_velocity);
    }
  }
}

void ChassisManager::move_chassis(
  const double& straight,
  const double& strafe,
  const double& turn,
  const double& sleep) const {
  this->move_chassis(straight, strafe, turn);
  pros::delay(sleep);
  this->move_chassis(0, 0, 0);
}

void ChassisManager::set_pid_consts(
  const pid_consts& pidconsts) {
  for (Motor *motor : _chassisMotors) {
    assert_not_nullptr(motor, "KRONOS::Motor");

    motor->set_pid_consts(pidconsts);
  }
}
}  // namespace kronos::base::managers
