/*
  Copyright 2025 Peter Duanmu

  @file assets/asserts.hpp

  Certain asserts for internal use that throw errors
*/

#ifndef KRONOS_ASSETS_ASSERTS_HPP_
#define KRONOS_ASSETS_ASSERTS_HPP_

#ifdef KRONOS_NOASSERTS
  #define NDEBUG
#endif

#include <string>

#include "KRONOS/base/devices.hpp"
#include "KRONOS/base/managers/taskmanager.hpp"
#include "KRONOS/base/managers/varmanager.hpp"

#include "KRONOS/internal/hotp.hpp"

namespace kronos::assets {
template <class T>
inline void assert_not_nullptr(const T* ptr, const std::string &expected) {
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
  const base::AbstractDevice* device, const device_types &type);
}  // namespace kronos::assets

#endif  // KRONOS_ASSETS_ASSERTS_HPP_
