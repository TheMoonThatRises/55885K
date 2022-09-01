/*
  @file assets/util.hpp

  Store helper functions
*/

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <chrono>

namespace KUTIL {
  /*
    Get time since epoch in milliseconds
  */
  inline long sinceEpoch() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch()).count();
  }
}

#endif
