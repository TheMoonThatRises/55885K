/*
  @file assets/statics.hpp

  Store helper functions
*/

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <string>

namespace KUtil {
  constexpr double KRONOS_MSDELAY = 20.0;
  constexpr double KRONOS_JOYSTICK_MOTOR_RATIO = 1.5748031496;

  constexpr std::string_view KRONOS_SD_CARD_PATH = "/usd/";

  enum side_color {
    S_RED, S_GREEN, S_NONE
  };
}

#endif
