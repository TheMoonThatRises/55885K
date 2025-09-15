/*
  Copyright 2025 Peter Duanmu

  @file base/managers/devicemanager.cpp

  Stores robot port devices
*/

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <ranges>

#include "KRONOS/base/managers/devicemanager.hpp"

#include "KRONOS/assets/asserts.hpp"
#include "KRONOS/assets/logger.hpp"

namespace kronos::base::managers {
using assets::assert_not_nullptr;
using assets::Logger;
using assets::device_types;
using assets::NoDeviceFoundError;

void DeviceManager::set(const std::string& name, AbstractDevice *device) {
  assert_not_nullptr(device, "unknown (" + name + ")");

  (void) _devices.emplace(name, std::move(device));

  auto port_name = _devices.at(name)->port().has_value()
    ? "' to port " + std::to_string(_devices.at(name)->port().value())
    : "";

  Logger::info(
    "Saved device of type " + std::to_string(_devices.at(name)->type()) +
    " with name '" + name + port_name);
}

/*
  Get all devices by a vector of keys

  @param dnames Vector of device keys

  @return A vector of the name of the device and device in a pair
*/
std::vector<std::pair<std::string, AbstractDevice*>>
DeviceManager::devices_by_key(const std::vector<std::string>& dnames) {
  std::vector<std::pair<std::string, AbstractDevice*>> filtered;

  for (const std::string& name : dnames) {
    if (get_device<AbstractDevice>(name)) {
      (void) filtered.emplace_back(name, get_device<AbstractDevice>(name));
    }
  }

  return filtered;
}

/*
  Gets all devices in stored

  @return Vector of all devices in the form of an AbstractDevice pointer
*/
std::vector<AbstractDevice*> DeviceManager::get_all() {
  std::vector<AbstractDevice*> devices(_devices.size());

  auto kv = std::views::values(_devices);

  (void) std::transform(
    kv.begin(),
    kv.end(),
    devices.begin(),
    [](const auto& device) {
      return device.get();
    });

  return devices;
}

/*
  Gets device stored

  @param name Name of sign of the device

  @return The device requested as an AbstractDevice pointer
*/
template <class Device>
Device* DeviceManager::get_device(const std::string& name) {
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
template <class Device>
Device* DeviceManager::get_device(const char& port) {
  for (const auto& [name, device] : _devices) {
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
std::vector<AbstractDevice*> DeviceManager::get_multiple_devices(
  const std::vector<std::string>& devices) {
  std::vector<AbstractDevice*> deviceVector(devices.size());

  (void) std::transform(
    devices.begin(),
    devices.end(),
    deviceVector.begin(),
    [&](const std::string& device) {
      return get_device<AbstractDevice>(device);
    });

  return deviceVector;
}

/*
  Gets all devices of the same type

  @param type The type of the device to get

  @return A vector of the device type
*/
template <class Device>
std::vector<Device*> DeviceManager::get_all_device_type(
  const device_types& type) {
  std::vector<Device*> deviceVector;

  for (const auto& [name, device] : _devices) {
    if (device->is_type(type)) {
      deviceVector.push_back(get_device<Device>(name));
    }
  }

  return deviceVector;
}

template AbstractDevice*
DeviceManager::get_device<AbstractDevice>(const std::string& name);
template Button*
DeviceManager::get_device<Button>(const std::string& name);
template Color*
DeviceManager::get_device<Color>(const std::string& name);
template Controller*
DeviceManager::get_device<Controller>(const std::string& name);
template Imu*
DeviceManager::get_device<Imu>(const std::string& name);
template Motor*
DeviceManager::get_device<Motor>(const std::string& name);
template PIDDevice*
DeviceManager::get_device<PIDDevice>(const std::string& name);
template Piston*
DeviceManager::get_device<Piston>(const std::string& name);
template Proximity*
DeviceManager::get_device<Proximity>(const std::string& name);
template Rotation*
DeviceManager::get_device<Rotation>(const std::string& name);
template Vision*
DeviceManager::get_device<Vision>(const std::string& name);
template GPS*
DeviceManager::get_device<GPS>(const std::string& name);
template LineTracker*
DeviceManager::get_device<LineTracker>(const std::string& name);

template AbstractDevice*
DeviceManager::get_device<AbstractDevice>(const char& port);
template Button*
DeviceManager::get_device<Button>(const char& port);
template Color*
DeviceManager::get_device<Color>(const char& port);
template Controller*
DeviceManager::get_device<Controller>(const char& port);
template Imu*
DeviceManager::get_device<Imu>(const char& port);
template Motor*
DeviceManager::get_device<Motor>(const char& port);
template PIDDevice*
DeviceManager::get_device<PIDDevice>(const char& port);
template Piston*
DeviceManager::get_device<Piston>(const char& port);
template Proximity*
DeviceManager::get_device<Proximity>(const char& port);
template Rotation*
DeviceManager::get_device<Rotation>(const char& port);
template Vision*
DeviceManager::get_device<Vision>(const char& port);
template GPS*
DeviceManager::get_device<GPS>(const char& port);
template LineTracker*
DeviceManager::get_device<LineTracker>(const char& port);

template std::vector<AbstractDevice*>
DeviceManager::get_all_device_type<AbstractDevice>(const device_types& type);
template std::vector<Button*>
DeviceManager::get_all_device_type<Button>(const device_types& type);
template std::vector<Color*>
DeviceManager::get_all_device_type<Color>(const device_types& type);
template std::vector<Controller*>
DeviceManager::get_all_device_type<Controller>(const device_types& type);
template std::vector<Imu*>
DeviceManager::get_all_device_type<Imu>(const device_types& type);
template std::vector<Motor*>
DeviceManager::get_all_device_type<Motor>(const device_types& type);
template std::vector<PIDDevice*>
DeviceManager::get_all_device_type<PIDDevice>(const device_types& type);
template std::vector<Piston*>
DeviceManager::get_all_device_type<Piston>(const device_types& type);
template std::vector<Proximity*>
DeviceManager::get_all_device_type<Proximity>(const device_types& type);
template std::vector<Rotation*>
DeviceManager::get_all_device_type<Rotation>(const device_types& type);
template std::vector<Vision*>
DeviceManager::get_all_device_type<Vision>(const device_types& type);
template std::vector<GPS*>
DeviceManager::get_all_device_type<GPS>(const device_types& type);
template std::vector<LineTracker*>
DeviceManager::get_all_device_type<LineTracker>(const device_types& type);
}  // namespace kronos::base::managers
