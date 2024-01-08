/*
  Copyright 2024 Peter Duanmu

  @file kronos.hpp

  Includes all of the KRONOS dependencies and custom functions
*/

#ifndef KRONOS_KRONOS_HPP_
#define KRONOS_KRONOS_HPP_
  // These flags should be used only when testing code.
  // They will either throw errors or come at a performance cost.
  // #define KRONOS_SAFETY_CHECKS
  // #define KRONOS_STRICT_DEVICE_GETTER
  // #define KRONOS_STRICT_MUTEX

  #define KRONOS_LOG_COUT
  #define KRONOS_LOG_FILE
  #define KRONOS_DEVICE_USE_MUTEX

  /*
    Include KRONOS libraries
  */
  #include "base/robot.hpp"  // Main robot class
  #include "base/devices.hpp"  // Devices able to be added to the robot

  #include "assets/errortypes.hpp"  // Errors KRONOS will throw

  #include "assets/logger.hpp"  // Logger class

  #include "base/user/caster.hpp"  // Group of dynamic cast functions
#endif  // KRONOS_KRONOS_HPP_
