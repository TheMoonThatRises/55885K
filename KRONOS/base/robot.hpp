/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "devicemanager.hpp"

namespace KRONOS {
  class Robot : public DeviceManager {
    public:
      /*
        Adds a device to the robot

        @param name Name to pair with the device
        @param device Device to set

        @return Self class
      */
      inline Robot& addDevice(const std::string &name, IOKParent *device) {
        set(name, device);

        return *this;
      }
  };
}

#endif
