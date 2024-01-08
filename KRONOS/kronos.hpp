/*
  Copyright 2024 Peter Duanmu

  @file kronos.hpp

  Includes all of the KRONOS dependencies and custom functions
*/

#ifndef KRONOS_KRONOS_HPP_
#define KRONOS_KRONOS_HPP_
  // Set macros for production code
  #ifdef KRONOS_PRODUCTION
    #define NDEBUG

    #define KRONOS_NOASSERTS

    #undef KRONOS_SAFETY_CHECKS
    #undef KRONOS_STRICT_DEVICE_GETTER
  #endif

  /*
    Include KRONOS libraries
  */
  #include "base/robot.hpp"  // Main robot class
  #include "base/devices.hpp"  // Devices able to be added to the robot

  #include "assets/errortypes.hpp"  // Errors KRONOS will throw

  #include "assets/logger.hpp"  // Logger class

  #include "base/user/caster.hpp"  // Group of dynamic cast functions
#endif  // KRONOS_KRONOS_HPP_
