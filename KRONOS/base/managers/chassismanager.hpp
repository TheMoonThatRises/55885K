/*
  @file base/managers/chassismanager.hpp

  Chassis manager for the KRONOS API
*/

#ifndef _CHASSISMANAGER_HPP_
#define _CHASSISMANAGER_HPP_

#include "base/devices.hpp"

#include <algorithm>
#include <vector>

namespace KRONOS {
  class ChassisManager {
    protected:
      std::vector<Motor*> _chassisMotors;

      /*
        Set chassis motors

        @param motors Vector of motor pointer
      */
      inline void set(const std::vector<Motor*> &motors) {
        _chassisMotors = motors;
      }
    public:
      /*
        Move the chassis

        @param straight
        @param strafe
        @param turn
      */
      inline void moveChassis(const double &straight, const double &strafe, const double &turn) {
        for (Motor *motor : _chassisMotors) {
          std::vector<device_face> reverseStrafe {K_NORTHWEST, K_SOUTHEAST};

          double mturn = motor->is_reversed() ? -turn : turn;
          double mstrafe = (std::find(reverseStrafe.begin(), reverseStrafe.end(), motor->facing()) != reverseStrafe.end()) ? strafe : -strafe;
          motor->move_velocity((straight + mstrafe + mturn) * JOYSTICK_MOTOR_RATIO);
        }
      }
  };
}

#endif
