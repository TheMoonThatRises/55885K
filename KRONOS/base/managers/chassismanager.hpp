/*
  @file base/managers/chassismanager.hpp

  Chassis manager for the KRONOS API
*/

#ifndef _CHASSISMANAGER_HPP_
#define _CHASSISMANAGER_HPP_

#include "assets/logger.hpp"

#include "base/devices.hpp"

#include <algorithm>
#include <vector>

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
      ChassisManager(): PID(KExtender::P_NONE, {}, {}) {}

      /*
        Move the chassis

        @param straight
        @param strafe
        @param turn
      */
      inline void move_chassis(const double &straight, const double &strafe, const double &turn) const {
        for (Motor *motor : _chassisMotors) {
          const double mstraight = (motor->facing() >= K_NORTHEAST && motor->facing() <= K_SOUTHEAST)  ? -straight : straight;
          const double mstrafe = (motor->facing() >= K_SOUTHEAST && motor->facing() <= K_SOUTHWEST) ? -strafe : strafe;

          const double move_value = (mstraight + mstrafe + turn) * KUtil::KRONOS_JOYSTICK_MOTOR_RATIO;

          if (_use_pid) {
            motor->move_velocity_pid(move_value);
          } else {
            motor->move_velocity(move_value);
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
      inline void move_chassis(const double &straight, const double &strafe, const double &turn, const double &sleep) const {
        this->move_chassis(straight, strafe, turn);
        pros::delay(sleep);
        this->move_chassis(0, 0, 0);
      }
  };
}

#endif
