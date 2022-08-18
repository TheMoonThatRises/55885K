/*
  @file base/devicemanager.hpp

  Stores robot port devices
*/

#ifndef _DEVICEMANAGER_HPP_
#define _DEVICEMANAGER_HPP_

#include <map>
#include <vector>

#include "devices.hpp"

namespace KRONOS {
  class DeviceManager {
    private:
      std::map<std::string, Button> buttons;
      std::map<std::string, Motor> motors;
      std::map<std::string, Piston> pistons;
      std::map<std::string, Proximity> proximities;
      std::map<std::string, Vision> visions;

      Controller controllers[2];
    public:
      /*
        Sets device to the appropriate device map

        @param name Name of the device
        @param device

        @return Success or not
      */
      template <class D>
      inline bool set(const std::string &name, const D &device) {
        std::map<std::string, D> addMap;

        switch (device.classname()) {
          case BUTTON:
            addMap = buttons;
            break;
          case MOTOR:
            addMap = motors;
            break;
          case PISTON:
            addMap = pistons;
            break;
          case PROXIMITY:
            addMap = proximities;
            break;
          case VISION:
            addMap = visions;
            break;
          case CONTROLLER:
            if (device.id() == pros::E_CONTROLLER_MASTER) {
              controllers[0] = std::move(device);
            } else {
              controllers[1] = std::move(device);
            }

            return true;
        }

        if (addMap.contains(name)) {
          std::cout << device.classname() << " map already contains key: " << name;

          return false;
        } else {
          addMap.insert({name, std::move(device)});

          return true;
        }
      }
  };
}

#endif
