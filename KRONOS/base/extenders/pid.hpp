/*
  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef _PID_HPP_
#define _PID_HPP_

#include "pros/rtos.hpp"

#include <cmath>

namespace KPID {
  enum pid_exit_conditions {
    error, time
  };

  struct pid_consts {
    double errormargin = 0.1,
           timeconstraint = 5000.0,
           kP = 0.5,
           kI = 0,
           kD = 0.4;
  };

  class PID {
    private:
      double _starttime = __DBL_MIN__;
    protected:
      const pid_exit_conditions _exitcondition;
      const pid_consts _pidconsts;
      double _previous;

      /*
        Get PID tuned value for target

        @param target Target position of the motor
        @param current Current position of the motor

        @return Value to set
      */
      inline double pid(const double &target, const double &current) {
        if (_exitcondition == time && _starttime == __DBL_MIN__) {
          _starttime = pros::millis();
        }

        const double output = (_pidconsts.kP * (target - current)) + (_pidconsts.kD * (_previous - current));

        _previous = current;

        if (_exitcondition == error && fabs(target - output) <= _pidconsts.errormargin) {
          return 0;
        } else if (_exitcondition == time && pros::millis() - _starttime >= 0) {
          _starttime = __DBL_MIN__;
          return 0;
        } else {
          return output;
        }
      }
    public:
      /*
        @param exitcondition
        @param pidconsts
      */
      inline explicit PID(const pid_exit_conditions &exitcondition, const pid_consts &pidconsts) : _exitcondition(exitcondition), _pidconsts(pidconsts) {}
  };
}

#endif
