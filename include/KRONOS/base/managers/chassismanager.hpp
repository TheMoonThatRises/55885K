/*
  Copyright 2025 Peter Duanmu

  @file base/managers/chassismanager.hpp

  Chassis manager for the KRONOS API
*/

#ifndef KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_

#include <vector>

#include "KRONOS/base/devices.hpp"

namespace kronos::base::managers {
class ChassisManager : protected extenders::PID {
 private:
    std::vector<Motor*> _chassisMotors;
    std::vector<Rotation*> _odomSensors;

    bool _use_pid = false;

 protected:
    /*
      Set chassis motors

      @param motors Vector of motor pointers
    */
    void set_motors(const std::vector<Motor*>& motors);

    /*
      Set odom sensors

      @param odoms
    */
    void set_odoms(const std::vector<Rotation*>& odoms);

    /*
      Set whether to use pid for chassis
    */
    void use_pid(const bool& use);

 public:
    ChassisManager();

    /*
      Move the chassis

      @param straight
      @param strafe
      @param turn
    */
    void move_chassis(
      const double& straight,
      const double& strafe,
      const double& turn) const;

    /*
      Move the chassis

      @param straight
      @param strafe
      @param turn
      @param sleep
    */
    void move_chassis(
      const double& straight,
      const double& strafe,
      const double& turn,
      const double& sleep) const;

    /*

    */
    void set_pid_consts(const assets::pid_consts& pidconsts) override;
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_CHASSISMANAGER_HPP_
