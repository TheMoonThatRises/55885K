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
    P_ERROR, P_TIME
  };

  struct pid_consts {
    double errormargin = 0.1,
           timeconstraint = 5000.0,
           kP = 0.5,
           kI = 0.1,
           kD = 0.4;
  };

  class PID {
    private:
      double _starttime = __DBL_MIN__;
      double _previousError, _integral;
    protected:
      const pid_exit_conditions _exitcondition;
      const pid_consts _pidconsts;

      inline void reset() {
        _starttime = __DBL_MIN__;
        _previousError = 0;
        _integral = 0;
      }

      /*
        Get PID tuned value for target

        @param target Target position of the motor
        @param current Current position of the motor

        @return Value to set
      */
      inline double pid(const double &target, const double &current) {
        if (_exitcondition == P_TIME && _starttime == __DBL_MIN__) {
          _starttime = pros::millis();
        }

        const double error = target - current;
        _integral += (error + _previousError) / 2;

        const double output = (_pidconsts.kP * (target - current)) + (_pidconsts.kI * _integral) + (_pidconsts.kD * (error - _previousError));

        _previousError = error;

        if (_exitcondition == P_ERROR && fabs(target - output) <= _pidconsts.errormargin) {
          reset();
          return 0;
        } else if (_exitcondition == P_TIME && pros::millis() - _starttime >= 0) {
          reset();
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
