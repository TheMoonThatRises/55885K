/*
  @file assets/util.hpp

  Store helper functions
*/

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <chrono>

namespace KUtil {
  #define MSDELAY 20.0
  #define JOYSTICK_MOTOR_RATIO 1.5748031496

  /*
    Get time since epoch in milliseconds
  */
  inline std::chrono::high_resolution_clock::time_point sinceEpoch() {
    return std::chrono::high_resolution_clock::now();
  }
}

#endif
