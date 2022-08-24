/*
  @file base/managers/devicemanager.hpp

  Stores robot port devices
*/

#ifndef _DEVICEMANAGER_HPP_
#define _DEVICEMANAGER_HPP_

#include <algorithm>
#include <map>
#include <vector>

#include "base/devices.hpp"

namespace KRONOS {
  class DeviceManager {
    private:
      std::map<std::string, AbstractDevice*> devices;
    protected:
      /*
        Sets device with name tag

        @param name Name to pair with the device
        @param device Device to set
      */
      inline virtual void setDevice(const std::string &name, AbstractDevice *device) {
        devices.insert({name, device});
      }

      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested
      */
      inline virtual AbstractDevice* getDevice(const std::string &name) {
        return devices.find(name) != devices.end() ? devices.at(name) : nullptr;
      }

      /*
        Get all devices by a vector of keys

        @param dnames Vector of keys

        @return A vector of devices
      */
      inline std::vector<std::pair<std::string, AbstractDevice*>> valuesByKeys(const std::vector<std::string> &dnames) {
        std::vector<std::pair<std::string, AbstractDevice*>> filtered;

        for (const std::string &name : dnames)
          if (getDevice(name))
            filtered.push_back({name, getDevice(name)});

        return filtered;
      }
  };
}

#endif
