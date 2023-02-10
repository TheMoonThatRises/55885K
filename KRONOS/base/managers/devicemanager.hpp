/*
  @file base/managers/devicemanager.hpp

  Stores robot port devices
*/

#ifndef _DEVICEMANAGER_HPP_
#define _DEVICEMANAGER_HPP_

#include "assets/logger.hpp"

#include "base/devices.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <ranges>
#include <vector>

namespace KRONOS {
  class DeviceManager {
    private:
      std::map<std::string, std::unique_ptr<AbstractDevice>> _devices;
    protected:
      /*
        Sets device with name tag

        @param name Name to pair with the device
        @param device Device to set
      */
      inline void set(const std::string &name, AbstractDevice *device) {
        _devices.emplace(name, std::move(device));

        KLog::Log::info("Saved device of type " + std::to_string(device->type()) + " with name '" + name + (device->port().has_value() ? "' to port " + std::to_string(device->port().value()) : ""));
      }

      /*
        Get all devices by a vector of keys

        @param dnames Vector of device keys

        @return A vector of the name of the device and device in a pair
      */
      inline std::vector<std::pair<std::string, AbstractDevice*>> devices_by_key(const std::vector<std::string> &dnames) {
        std::vector<std::pair<std::string, AbstractDevice*>> filtered;

        for (const std::string &name : dnames)
          if (get_device<AbstractDevice>(name))
            filtered.emplace_back(name, get_device<AbstractDevice>(name));

        return filtered;
      }
    public:
      /*
        Gets all devices in stored

        @return Vector of all devices in the form of an AbstractDevice pointer
      */
      inline std::vector<AbstractDevice*> get_all() {
        std::vector<AbstractDevice*> devices;
        auto kv = std::views::values(_devices);

        std::transform(kv.begin(), kv.end(), back_inserter(devices), [](const auto &device) { return device.get(); });

        return devices;
      }

      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested as an AbstractDevice pointer
      */
      template <class Device = AbstractDevice>
      inline Device* get_device(const std::string &name) {
        if (_devices.find(name) == _devices.end()) {
          #ifdef KRONOS_STRICT_DEVICE_GETTER
            throw new NoDeviceFoundError(name);
          #else
            return nullptr;
          #endif
        } else {
          return dynamic_cast<Device*>(_devices.at(name).get());
        }
      }

      /*
        Gets device stored

        @param port Port device is on

        @return The device requested as a Device pointer
      */
      template <class Device = AbstractDevice>
      inline Device* get_device(const char &port) {
        for (const auto &[name, device] : _devices)
          if (device->port() == port)
            return dynamic_cast<Device*>(device.get());

        #ifdef KRONOS_STRICT_DEVICE_GETTER
          throw new NoDeviceFoundError(port);
        #else
          return nullptr;
        #endif
      }

      /*
        Get multiple devices by a vector

        @param devices Vector of device names

        @return A vector of AbstractDevice pointers
      */
      inline std::vector<AbstractDevice*> get_multiple_devices(const std::vector<std::string> &devices) {
        std::vector<AbstractDevice*> deviceVector;

        for (const std::string &device : devices) {
          deviceVector.push_back(get_device<AbstractDevice>(device));
        }

        return deviceVector;
      }
  };
}

#endif
