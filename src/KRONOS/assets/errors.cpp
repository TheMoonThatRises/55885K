/*
  Copyright 2025 Peter Duanmu

  @file assets/errors.cpp

  Stores custom error classes for KRONOS
*/

#include <string>

#include "KRONOS/assets/errors.hpp"

#include "KRONOS/assets/logger.hpp"

#include "pros/apix.h"

namespace kronos::assets {
using assets::Logger;
using assets::side_color;

AbstractKronosError::AbstractKronosError(const std::string& message)
: std::runtime_error(message) {
  Logger::error(message);
}

ColorAlreadySetError::ColorAlreadySetError(const side_color& previous)
: AbstractKronosError(
  "Side color already set to '" + std::to_string(previous) + "'"
) {}

InvalidElevatedVariableAccess::InvalidElevatedVariableAccess(
  const std::string& key)
: AbstractKronosError(
  "Attempted to access variable '" + key + "' with improper verification"
) {}

NoDeviceFoundError::NoDeviceFoundError(const std::string& devicename)
: AbstractKronosError(
  "No device registered with name: '" + devicename + "'"
) {}

NoDeviceFoundError::NoDeviceFoundError(const char& port)
: AbstractKronosError(
  "No device found at port: '" + std::to_string(port) + "'"
) {}

NullPointerError::NullPointerError(const std::string& expected)
: AbstractKronosError(
  "Expected pointer of '" + expected + "' but found nullptr"
) {}

PortOccupiedError::PortOccupiedError(const char& port)
: AbstractKronosError(
  "Port is already occupied: '" + std::to_string(port) + "'"
) {}

UnexpectedDeviceFoundError::UnexpectedDeviceFoundError(
  const pros::c::v5_device_e_t& expected,
  const device_types& got,
  const char& port)
: AbstractKronosError(
  "Expected device type '" + std::to_string(expected) +
  "' but got '" + std::to_string(got) +
  "'" + " at port '" + std::to_string(port) + "'"
) {}

UnexpectedDeviceTypeError::UnexpectedDeviceTypeError(
  const device_types& expected,
  const device_types& got)
: AbstractKronosError(
  "Expected device type '" + std::to_string(expected) +
  "' but got '" + std::to_string(got) + "' when casting"
) {}

UnsuccessfulMutexTake::UnsuccessfulMutexTake()
: AbstractKronosError("Unsuccessful mutex take: " + errno) {}
}  // namespace kronos::assets
