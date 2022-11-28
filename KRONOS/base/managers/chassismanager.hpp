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
  class ChassisManager {
    protected:
      std::vector<Motor*> _chassisMotors;

      /*
        Set chassis motors

        @param motors Vector of motor pointer
      */
      inline void set(const std::vector<Motor*> &motors) {
        _chassisMotors = motors;

        std::vector<char> ports;
        std::transform(motors.begin(), motors.end(), ports.begin(), [](Motor* motor) { return motor->port(); });

        KLog::Log::info("Setting chassis motors to ports " + std::string(ports.begin(), ports.end()));
      }
    public:
      /*
        Move the chassis

        @param straight
        @param strafe
        @param turn
      */
      inline void move_chassis(const double &straight, const double &strafe, const double &turn) {
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
