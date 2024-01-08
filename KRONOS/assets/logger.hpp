/*
  Copyright 2024 Peter Duanmu

  @file assets/logger.hpp

  Logger for the KRONOS library
*/

#ifndef KRONOS_ASSETS_LOGGER_HPP_
#define KRONOS_ASSETS_LOGGER_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "assets/statics.hpp"
#include "assets/uuid.hpp"

#include "pros/rtos.hpp"

namespace KLog {
enum log_types {
  L_ERROR, L_INFO, L_WARNING
};

class Log {
 private:
    inline static bool usd_installed = true;

 public:
    /*
      Default log function

      @param type Log type
      @param message Message to enter into the logger
    */
    static inline void log(const log_types &type, const std::string &message) {
      std::string logmessage = std::to_string(pros::micros()) + " ";

      switch (type) {
        case log_types::L_ERROR:
          logmessage += "[ERROR]";
          break;
        case log_types::L_INFO:
          logmessage += "[INFO]";
          break;
        case log_types::L_WARNING:
          logmessage += "[WARN]";
          break;
        default:
          logmessage += "[MESSAGE]";
      }

      logmessage += ": " + message + "\n";

      #ifdef KRONOS_LOG_COUT
        std::cout << logmessage;
      #endif

      #ifdef KRONOS_LOG_FILE
        if (pros::usd::is_installed()) {
          static std::string fileName;
          std::ofstream logFile;

          if (fileName.empty()) {
            fileName = std::string(KUtil::KRONOS_SD_CARD_PATH) +
                       KUUID::UUIDGenerator().generate_uuid() + ".log";
          }

          logFile.open(fileName, std::ios_base::app);

          logFile << logmessage;

          logFile.close();
        } else if (usd_installed) {
          usd_installed = false;
          warn(
            "MicroSD card directory '" +
            std::string(KUtil::KRONOS_SD_CARD_PATH) + "' not found");
        }
      #endif
    }

    /*
      Error logger function

      @param message Error message
    */
    static inline void error(const std::string &message) {
      log(log_types::L_ERROR, message);
    }

    /*
      Info logger function

      @param message Error message
    */
    static inline void info(const std::string &message) {
      log(log_types::L_INFO, message);
    }

    /*
      Warning logger function

      @param message Error message
    */
    static inline void warn(const std::string &message) {
      log(log_types::L_WARNING, message);
    }
};
}  // namespace KLog

#endif  // KRONOS_ASSETS_LOGGER_HPP_
