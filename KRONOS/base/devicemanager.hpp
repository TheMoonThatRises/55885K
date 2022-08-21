/*
  @file base/devicemanager.hpp

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
      std::map<std::string, IOKParent*> devices;
    protected:
      /*
        Sets device with name tag

        @param name Name to pair with the device
        @param device Device to set
      */
      inline void set(const std::string &name, IOKParent *device) {
        devices.insert(std::make_pair(name, device));
      }

      /*
        Gets device stored

        @param name Name of sign of the device

        @return The device requested
      */
      inline IOKParent* get(const std::string &name) {
        return devices.at(name);
      }

      /*
        Get all devices by a vector of keys

        @param dnames Vector of keys

        @return A vector of devices
      */
      inline std::vector<IOKParent*> valuesByKeys(const std::vector<std::string> &dnames) {
        std::vector<IOKParent*> filtered;

        for (auto [key, value] : devices)
          if (std::find(dnames.begin(), dnames.end(), key) != dnames.end())
            filtered.push_back(value);

        return filtered;
      }
  };
}

#endif
