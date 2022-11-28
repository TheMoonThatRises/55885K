/*
  @file assets/logger.hpp

  Logger for the KRONOS library
*/

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "pros/rtos.hpp"

#include <iostream>
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
        std::cout << logmessage;
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
