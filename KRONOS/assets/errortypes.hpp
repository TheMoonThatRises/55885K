/*
  @file assets/errortypes.hpp

  Stores custom error classes for KRONOS
*/

#ifndef _ERRORTYPES_HPP_
#define _ERRORTYPES_HPP_

#include "assets/devicestructs.hpp"
#include "assets/logger.hpp"
#include "assets/statics.hpp"

#include "pros/apix.h"

#include <system_error>

namespace KRONOS {
  class AbstractKronosError : public std::runtime_error {
    public:
      explicit AbstractKronosError(const std::string &message) : std::runtime_error(message) {
        KLog::Log::error(message);
      };
  };

  class ColorAlreadySetError : public AbstractKronosError {
    public:
      explicit ColorAlreadySetError(const KUtil::side_color &previous) : AbstractKronosError("Side color already set to '" + std::to_string(previous) + "'") {};
  };

  class NoDeviceFoundError : public AbstractKronosError {
    public:
      explicit NoDeviceFoundError(const std::string &devicename) : AbstractKronosError("No device registered with name: '" + devicename + "'") {};
      explicit NoDeviceFoundError(const char &port) : AbstractKronosError("No device found at port: '" + std::to_string(port) + "'") {};
  };

  class PortOccupiedError : public AbstractKronosError {
    public:
      explicit PortOccupiedError(const char &port) : AbstractKronosError("Port is already occupied: '" + std::to_string(port) + "'") {};
  };

  class UnexpectedDeviceFoundError : public AbstractKronosError {
    public:
      explicit UnexpectedDeviceFoundError(const pros::c::v5_device_e_t &expected, const device_types& got, const char& port) : AbstractKronosError("Expected device type '" + std::to_string(expected) + "' but got '" + std::to_string(got) + "'" + " at port '" + std::to_string(port) + "'") {};
  };

  class UnexpectedDeviceTypeError : public AbstractKronosError {
    public:
      explicit UnexpectedDeviceTypeError(const device_types &expected, const device_types &got) : AbstractKronosError("Expected device type '" + std::to_string(expected) + "' but got '" + std::to_string(got) + "' when casting") {};
  };
}

#endif