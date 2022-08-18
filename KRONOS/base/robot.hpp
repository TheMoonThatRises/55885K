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

        @param name Name of the device
        @param device

        @return Self class
      */
      template <class D>
      inline Robot& addDevice(const std::string &name, const D &device) {
        set(name, device);

        return *this;
      }
  };
}

#endif
