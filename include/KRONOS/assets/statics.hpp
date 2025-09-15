/*
  Copyright 2025 Peter Duanmu

  @file assets/statics.hpp

  Store helper functions
*/

#ifndef KRONOS_ASSETS_STATICS_HPP_
#define KRONOS_ASSETS_STATICS_HPP_

#include <cstdint>
#include <string>

namespace kronos::assets {
inline constexpr uint32_t KRONOS_MSDELAY = 20;
inline constexpr double KRONOS_JOYSTICK_MOTOR_RATIO = 1.5748031496;

inline constexpr std::string_view KRONOS_SD_CARD_PATH = "/usd/";

enum side_color {
  S_RED = 1, S_BLUE
};
}  // namespace kronos::assets

#endif  // KRONOS_ASSETS_STATICS_HPP_
