/*
  Copyright 2025 Peter Duanmu

  @file assets/errors.hpp

  Stores custom error classes for KRONOS
*/

#ifndef KRONOS_ASSETS_ERRORS_HPP_
#define KRONOS_ASSETS_ERRORS_HPP_

#include <string>

#include "KRONOS/assets/structs.hpp"

namespace kronos::assets {
class AbstractKronosError : public std::runtime_error {
 public:
    explicit AbstractKronosError(const std::string& message);
};

class ColorAlreadySetError : public AbstractKronosError {
 public:
    explicit ColorAlreadySetError(const side_color& previous);
};

class InvalidElevatedVariableAccess : public AbstractKronosError {
 public:
  explicit InvalidElevatedVariableAccess(const std::string& key);
};

class NoDeviceFoundError : public AbstractKronosError {
 public:
    explicit NoDeviceFoundError(const std::string& devicename);
    explicit NoDeviceFoundError(const char& port);
};

class NullPointerError : public AbstractKronosError {
 public:
    explicit NullPointerError(const std::string& expected);
};

class PortOccupiedError : public AbstractKronosError {
 public:
    explicit PortOccupiedError(const char& port);
};

class UnexpectedDeviceFoundError : public AbstractKronosError {
 public:
    explicit UnexpectedDeviceFoundError(
      const pros::c::v5_device_e_t& expected,
      const device_types& got,
      const char& port);
};

class UnexpectedDeviceTypeError : public AbstractKronosError {
 public:
    explicit UnexpectedDeviceTypeError(
      const device_types& expected,
      const device_types& got);
};

class UnsuccessfulMutexTake : public AbstractKronosError {
 public:
    UnsuccessfulMutexTake();
};
}  // namespace kronos::assets

#endif  // KRONOS_ASSETS_ERRORS_HPP_
