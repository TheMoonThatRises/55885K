/*
  Copyright 2024 Peter Duanmu

  @file base/managers/chassismanager.hpp

  Chassis manager for the KRONOS API
*/

#ifndef KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_

#include <algorithm>
#include <vector>

#include "assets/asserts.hpp"
#include "assets/logger.hpp"

#include "base/devices.hpp"

namespace KRONOS {
class ChassisManager: protected KExtender::PID {
 private:
    std::vector<Motor*> _chassisMotors;

    bool _use_pid = false;

 protected:
    /*
      Set chassis motors

      @param motors Vector of motor pointers
    */
    inline void set(const std::vector<Motor*> &motors) {
      _chassisMotors = motors;
    }

    /*
      Set whether to use pid for chassis
    */
    inline void use_pid(const bool &use) {
      _use_pid = use;
    }

 public:
    ChassisManager() : PID(KExtender::P_NONE, {}, {}) {}

    /*
      Move the chassis

      @param straight
      @param strafe
      @param turn
    */
    inline void move_chassis(
      const double &straight,
      const double &strafe,
      const double &turn) const {
      for (Motor *motor : _chassisMotors) {
        assert_not_nullptr(motor, "KRONOS::Motor");

        const double mstraight =
          (motor->facing() >= K_NORTHEAST && motor->facing() <= K_SOUTHEAST)
            ? -straight
            : straight;
        const double mstrafe =
          (motor->facing() >= K_SOUTHEAST && motor->facing() <= K_SOUTHWEST)
            ? -strafe
            : strafe;

        const double target_velocity =
          (mstraight + mstrafe + turn) * KUtil::KRONOS_JOYSTICK_MOTOR_RATIO;

        motor->set_max_speed(target_velocity);

        if (_use_pid) {
          (void) motor->move_velocity_pid(target_velocity);
        } else {
          (void) motor->move_velocity(target_velocity);
        }
      }
    }

    /*
      Move the chassis

      @param straight
      @param strafe
      @param turn
      @param sleep
    */
    inline void move_chassis(
      const double &straight,
      const double &strafe,
      const double &turn,
      const double &sleep) const {
      this->move_chassis(straight, strafe, turn);
      pros::delay(sleep);
      this->move_chassis(0, 0, 0);
    }

    /*

    */
    inline void set_pid_consts(
      const KExtender::pid_consts &pidconsts) override {
      for (Motor *motor : _chassisMotors) {
        assert_not_nullptr(motor, "KRONOS::Motor");

        motor->set_pid_consts(pidconsts);
      }
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_
