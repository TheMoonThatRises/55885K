/*
  Copyright 2024 Peter Duanmu

  @file base/managers/devicemanager.hpp

  Stores robot port devices
*/

#ifndef KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_

#include <ranges>

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "assets/logger.hpp"

#include "base/devices.hpp"

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
      (void) _devices.emplace(name, std::move(device));

      auto port_name = _devices.at(name)->port().has_value()
        ? "' to port " + std::to_string(_devices.at(name)->port().value())
        : "";

      KLog::Log::info(
        "Saved device of type " + std::to_string(_devices.at(name)->type()) +
        " with name '" + name + port_name);
    }

    /*
      Get all devices by a vector of keys

      @param dnames Vector of device keys

      @return A vector of the name of the device and device in a pair
    */
    inline std::vector<std::pair<std::string, AbstractDevice*>> devices_by_key(
      const std::vector<std::string> &dnames) {
      std::vector<std::pair<std::string, AbstractDevice*>> filtered;

      for (const std::string &name : dnames) {
        if (get_device<AbstractDevice>(name)) {
          (void) filtered.emplace_back(name, get_device<AbstractDevice>(name));
        }
      }

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

      (void) std::transform(
        kv.begin(),
        kv.end(),
        back_inserter(devices),
        [](const auto &device) {
          return device.get();
        });

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
      for (const auto &[name, device] : _devices) {
        if (device->port() == port) {
          return dynamic_cast<Device*>(device.get());
        }
      }

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
    inline std::vector<AbstractDevice*> get_multiple_devices(
      const std::vector<std::string> &devices) {
      std::vector<AbstractDevice*> deviceVector;

      (void) std::transform(
        devices.begin(),
        devices.end(),
        deviceVector.begin(),
        [&](const std::string &device) {
          return get_device<AbstractDevice>(device);
        });

      return deviceVector;
    }

    /*
      Gets all devices of the same type

      @param type The type of the device to get

      @return A vector of the device type
    */
    template <class Device = AbstractDevice>
    inline std::vector<Device*> get_all_device_type(const device_types &type) {
      std::vector<Device*> deviceVector;

      for (const auto &[name, device] : _devices) {
        if (device->is_type(type)) {
          deviceVector.push_back(get_device<Device>(name));
        }
      }

      return deviceVector;
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_
