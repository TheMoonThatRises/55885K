/*
  @file kronos.hpp

  Includes all of the KRONOS dependencies and custom functions
*/

#ifndef _KRONOS_HPP_
#define _KRONOS_HPP_
  /*
    Include KRONOS libraries
  */
  #include "base/robot.hpp" // Main robot class
  #include "base/devices.hpp" // Devices able to be added to the robot

  #include "assets/errortypes.hpp" // Errors KRONOS will throw

  #include "assets/logger.hpp" // Logger class

  #include "base/user/caster.hpp" // Group of dynamic cast functions

  /*
    Include KRONOS plugins
  */
  #include "plugins.hpp"

#endif
