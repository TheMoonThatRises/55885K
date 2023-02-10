/*
  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef _PID_HPP_
#define _PID_HPP_

#include "pros/rtos.hpp"

#include <cmath>
#include <numeric>
#include <optional>

namespace KExtender {
  enum pid_exit_conditions {
    P_ERROR, P_NONE, P_TIME
  };

  struct pid_consts {
    double errormargin = 0.1,
           timeconstraint = 5000.0,
           minspeed = -600,
           maxspeed = 600,
           kP = 0.5,
           kI = 0.1,
           kD = 0.4;
  };

  class PID {
    private:
      std::optional<double> _starttime;
      double _previousError {}, _integral {};

      std::vector<double> _consistencyValues {};

      pid_exit_conditions _exitcondition;
      pid_consts _pidconsts;
    public:
      /*
        @param exitcondition
        @param pidconsts
      */
      inline explicit PID(const pid_exit_conditions &exitcondition, const pid_consts &pidconsts) : _exitcondition(exitcondition), _pidconsts(pidconsts) {}

      /*
        Get PID tuned value for target

        @param target Target position of the motor
        @param current Current position of the motor

        @return Value to set
      */
      inline double pid(const double &target, const double &current) {
        if (_exitcondition == P_TIME && !_starttime.has_value()) {
          _starttime = pros::millis();
        }

        const double error = target - current;
        _integral += (error + _previousError) / 2;

        const double output = (_pidconsts.kP * error) + (_pidconsts.kI * _integral) + (_pidconsts.kD * (error - _previousError));

        _previousError = error;

        if ((_exitcondition == P_ERROR && fabs(target - output) <= _pidconsts.errormargin) ||
            (_exitcondition == P_TIME && pros::millis() - _starttime.value() >= _pidconsts.timeconstraint)) {
          reset();
          return 0;
        } else {
          return std::min(std::max(output, _pidconsts.minspeed), _pidconsts.maxspeed);
        }
      }

      inline void add_consistency_value(const double &value) {
        _consistencyValues.push_back(value);

        if (_consistencyValues.size() > 10) {
          _consistencyValues.erase(_consistencyValues.begin());
        }
      }

      inline bool consistency(const double &compare) {
        const double result = std::reduce(_consistencyValues.begin(), _consistencyValues.end());

        return result >= compare - 10 && result <= compare + 10;
      }

      inline void reset() {
        _starttime.reset();
        _previousError = 0;
        _integral = 0;
      }

      inline void set_exit_condition(const pid_exit_conditions &exit) {
        _exitcondition = exit;
      }

      inline void set_pid_consts(const pid_consts &pidconsts) {
        _pidconsts = pidconsts;
      }

      inline void set_max_speed(const double &max) {
        _pidconsts.maxspeed = max;
      }
  };
}

#endif
