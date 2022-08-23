/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "base/devicemanager.hpp"
#include "pros/rtos.hpp"

#include <functional>

namespace KRONOS {
  class Robot : public DeviceManager {
    public:
      /*
        Adds a device to the robot

        @param name Name to pair with the device
        @param device Device to set

        @return Self class
      */
      inline Robot& addDevice(const std::string &name, AbstractDevice *device) {
        set(name, device);

        return *this;
      }

      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested
      */
      inline AbstractDevice* getDevice(const std::string &name) {
        return get(name);
      }

      /*
        Manipulate groups of devices at the same time with the same command

        @param dnames Vector of names for devices
        @param manipFunc Function call that controls the device
        @param delay Delay after the manipDevices has ran
      */
      inline void manipDevices(const std::vector<std::string> &dnames, std::function<void(std::pair<std::string, AbstractDevice*>)> manipFunc, int delay = 50) {
        for (std::pair<std::string, AbstractDevice*> device : valuesByKeys(dnames))
          manipFunc(device);

        pros::delay(delay);
      }
  };
}

#endif
