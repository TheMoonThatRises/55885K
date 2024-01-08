/*
  Copyright 2024 Peter Duanmu

  @file assets/statics.hpp

  Store helper functions
*/

#ifndef KRONOS_ASSETS_STATICS_HPP_
#define KRONOS_ASSETS_STATICS_HPP_

#include <string>

namespace KUtil {
constexpr uint32_t KRONOS_MSDELAY = 20;
constexpr double KRONOS_JOYSTICK_MOTOR_RATIO = 1.5748031496;

constexpr std::string_view KRONOS_SD_CARD_PATH = "/usd/";

enum side_color {
  S_RED = 1, S_BLUE
};
}  // namespace KUtil

#endif  // KRONOS_ASSETS_STATICS_HPP_
