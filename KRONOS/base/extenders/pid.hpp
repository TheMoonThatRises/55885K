/*
  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef _PID_HPP_
#define _PID_HPP_

#include <cmath>

namespace KPID {
  enum pid_exit_conditions {
    error, time
  };

  struct pid_consts {
    double errormargin = 0.1,
           kP = 0.5,
           kI = 0,
           kD =0.4;
  };

  class PID {
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
        const double output = (_pidconsts.kP * (target - current)) + (_pidconsts.kD * (_previous - current));

        _previous = current;

        if (_exitcondition == error && fabs(target - output) <= _pidconsts.errormargin) {
          return 0;
        } else {
          return output;
        }
      }

    public:
      /*
        @param exitconditions
        @param pidconsts
      */
      inline explicit PID(const pid_exit_conditions &exitcondition, const pid_consts &pidconsts) : _exitcondition(exitcondition), _pidconsts(pidconsts) {}
  };
}

#endif
