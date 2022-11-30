/*
  @file assets/logger.hpp

  Logger for the KRONOS library
*/

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "assets/util.hpp"
#include "assets/uuid.hpp"

#include "pros/rtos.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace KLog {
  enum log_types {
    error, info, warning
  };

  class Log {
    protected:
      /*
        Default log function

        @param type Log type
        @param message Message to enter into the logger
      */
      static inline void log(const log_types &type, const std::string &message) {
        std::string logmessage = std::to_string(pros::micros()) + " ";

        switch (type) {
          case log_types::error:
            logmessage += "[ERROR]";
            break;
          case log_types::info:
            logmessage += "[INFO]";
            break;
          case log_types::warning:
            logmessage += "[WARN]";
            break;
          default:
            logmessage += "[MESSAGE]";
        }

        logmessage += ": " + message + "\n";

        #ifdef LOG_COUT
          std::cout << logmessage;
        #endif

        #ifdef LOG_FILE
          if (std::filesystem::is_directory(SD_CARD_PATH)) {
            static std::string fileName;
            std::ofstream logFile;

            if (fileName.empty()) {
              fileName = SD_CARD_PATH + KUUID::UUIDGenerator().generate_uuid() + ".log";
            }

            logFile.open(fileName);

            logFile << logmessage;

            logFile.close();
          } else {
            throw new std::system_error(std::error_code(), "MicroSD card directory '" + std::string(SD_CARD_PATH) + "' not found");
          }
        #endif
      }
    public:
      /*
        Error logger function

        @param message Error message
      */
      static inline void error(const std::string &message) {
        log(log_types::error, message);
      }

      /*
        Info logger function

        @param message Error message
      */
      static inline void info(const std::string &message) {
        log(log_types::info, message);
      }

      /*
        Warning logger function

        @param message Error message
      */
      static inline void warn(const std::string &message) {
        log(log_types::warning, message);
      }
  };
}

#endif