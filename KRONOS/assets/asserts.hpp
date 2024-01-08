/*
  Copyright 2024 Peter Duanmu

  @file base/assets/asserts.hpp

  Certain asserts for internal use that throw errors
*/

#ifndef KRONOS_ASSETS_ASSERTS_HPP_
#define KRONOS_ASSETS_ASSERTS_HPP_

#ifdef KRONOS_NOASSERTS
  #define NDEBUG
#endif

#include <string>

#include "assets/errortypes.hpp"

#include "base/devices.hpp"

namespace KRONOS {
template <class T>
void assert_not_nullptr(const T* ptr, const std::string &expected) {
  #ifdef KRONOS_NOASSERTS
    (void) ptr;
    (void) expected;
  #else
    if (!ptr) {
      throw new NullPointerError(expected);
    }
  #endif
}

void assert_same_device(
  const AbstractDevice* device,
  const device_types &type) {
  #ifdef KRONOS_NOASSERTS
    (void) device;
    (void) type;
  #else
    assert_not_nullptr(device, std::to_string(type));

    if (device->type() != type) {
      throw new UnexpectedDeviceTypeError(device->type(), type);
    }
  #endif
}
}  // namespace KRONOS

#endif  // KRONOS_ASSETS_ASSERTS_HPP_
