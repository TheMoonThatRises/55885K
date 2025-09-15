/*
  Copyright 2025 Peter Duanmu

  @file assets/logger.hpp

  Logger for the KRONOS library
*/

#ifndef KRONOS_ASSETS_LOGGER_HPP_
#define KRONOS_ASSETS_LOGGER_HPP_

#include <string>

namespace kronos::assets {
enum log_types {
  L_ERROR, L_INFO, L_WARNING
};

class Logger {
 private:
    inline static bool usd_installed = true;

 public:
    static void log(const log_types& type, const std::string& message);
    static void error(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
};
}  // namespace kronos::assets

#endif  // KRONOS_ASSETS_LOGGER_HPP_
