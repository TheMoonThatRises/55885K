/*
  Copyright 2025 Peter Duanmu

  @file assets/asserts.cpp

  Certain asserts for internal use that throw errors
*/

#include <string>

#include "KRONOS/assets/asserts.hpp"

#include "KRONOS/assets/errors.hpp"

namespace kronos::assets {
using base::AbstractDevice;

void assert_same_device(
const AbstractDevice* device,
const device_types& type) {
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
}  // namespace kronos::assets
