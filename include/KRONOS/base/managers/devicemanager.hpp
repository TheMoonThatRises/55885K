/*
  Copyright 2025 Peter Duanmu

  @file base/managers/devicemanager.hpp

  Stores robot port devices
*/

#ifndef KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "KRONOS/base/devices.hpp"

namespace kronos::base::managers {
class DeviceManager {
 private:
    std::map<std::string, std::unique_ptr<AbstractDevice>> _devices;

 protected:
    /*
      Sets device with name tag

      @param name Name to pair with the device
      @param device Device to set
    */
    void set(const std::string& name, AbstractDevice *device);

    /*
      Get all devices by a vector of keys

      @param dnames Vector of device keys

      @return A vector of the name of the device and device in a pair
    */
    std::vector<std::pair<std::string, AbstractDevice*>> devices_by_key(
      const std::vector<std::string>& dnames);

 public:
    /*
      Gets all devices in stored

      @return Vector of all devices in the form of an AbstractDevice pointer
    */
    std::vector<AbstractDevice*> get_all();

    /*
      Gets device stored

      @param name Name of sign of the device

      @return The device requested as an AbstractDevice pointer
    */
    template <class Device = AbstractDevice>
    Device* get_device(const std::string& name);

    /*
      Gets device stored

      @param port Port device is on

      @return The device requested as a Device pointer
    */
    template <class Device = AbstractDevice>
    Device* get_device(const char& port);

    /*
      Get multiple devices by a vector

      @param devices Vector of device names

      @return A vector of AbstractDevice pointers
    */
    std::vector<AbstractDevice*> get_multiple_devices(
      const std::vector<std::string>& devices);

    /*
      Gets all devices of the same type

      @param type The type of the device to get

      @return A vector of the device type
    */
    template <class Device = AbstractDevice>
    std::vector<Device*> get_all_device_type(const assets::device_types& type);
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_DEVICEMANAGER_HPP_
