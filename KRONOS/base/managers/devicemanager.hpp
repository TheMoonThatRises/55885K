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
      std::map<std::string, AbstractDevice*> _devices;
    protected:
      /*
        Sets device with name tag

        @param name Name to pair with the device
        @param device Device to set
      */
      inline void set(const std::string &name, AbstractDevice *device) {
        _devices.insert({name, device});
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
            filtered.emplace_back(name, getDevice(name));

        return filtered;
      }
    public:
      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested
      */
      inline AbstractDevice* getDevice(const std::string &name) {
        return _devices.find(name) != _devices.end() ? _devices.at(name) : nullptr;
      }

      /*
        Get multiple devices by a vector

        @param devices Vector of device names

        @return A vector of AbstractDevice pointers
      */
      inline std::vector<AbstractDevice*> getMultipleDevices(const std::vector<std::string> &devices) {
        std::vector<AbstractDevice*> deviceVector;

        for (const std::string &device : devices)
          deviceVector.push_back(getDevice(device));

        return deviceVector;
      }
  };
}

#endif
