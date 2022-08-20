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
  };
}

#endif
