/*
  @file base/managers/devicemanager.hpp

  Stores robot port devices
*/

#ifndef _DEVICEMANAGER_HPP_
#define _DEVICEMANAGER_HPP_

#include "base/devices.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <vector>

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
        Gets all devices in stored

        @return Vector of all devices in the form of an AbstractDevice pointer
      */
      inline std::vector<AbstractDevice*> get_all() {
        auto kv = std::views::values(_devices);
        return { kv.begin(), kv.end() };
      }

      /*
        Get all devices by a vector of keys

        @param dnames Vector of device keys

        @return A vector of the name of the device and device in a pair
      */
      inline std::vector<std::pair<std::string, AbstractDevice*>> devices_by_key(const std::vector<std::string> &dnames) {
        std::vector<std::pair<std::string, AbstractDevice*>> filtered;

        for (const std::string &name : dnames)
          if (get_device(name))
            filtered.emplace_back(name, get_device(name));

        return filtered;
      }
    public:
      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested as an AbstractDevice pointer
      */
      inline AbstractDevice* get_device(const std::string &name) {
        return _devices.find(name) != _devices.end() ? _devices.at(name) : nullptr;
      }

      /*
        Gets device stored

        @param port Port device is on

        @return The device requested as an AbstractDevice pointer
      */
      inline AbstractDevice* get_device(const char &port) {
        for (const auto [name, device] : _devices)
          if (device->port() == port)
            return device;

        return nullptr;
      }

      /*
        Get multiple devices by a vector

        @param devices Vector of device names

        @return A vector of AbstractDevice pointers
      */
      inline std::vector<AbstractDevice*> get_multiple_devices(const std::vector<std::string> &devices) {
        std::vector<AbstractDevice*> deviceVector;

        for (const std::string &device : devices)
          deviceVector.push_back(get_device(device));

        return deviceVector;
      }
  };
}

#endif
