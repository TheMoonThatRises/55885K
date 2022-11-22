/*
  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef _PID_HPP_
#define _PID_HPP_

namespace KPID {
  enum pid_exit_conditions {
    error, time
  };

  struct pid_modifiables {
    double errormargin = 0.1,
           kP = 0.5,
           kI = 0,
           kD =0.4;
  };

  class PID {
    protected:
      const pid_exit_conditions _exitcondition;
      pid_modifiables _modifiables;
      double _previous;

      /*
        Get PID tuned value for target

        @param target Target position of the motor
        @param current Current position of the motor
      */
      inline double pid(const double &target, const double &current) {
        const double output = (_modifiables.kP * (target - current)) + (_modifiables.kD * (_previous - current));

        _previous = current;

        if (_exitcondition == error && target - output <= _modifiables.errormargin) {
          return 0;
        } else {
          return output;
        }
      }

    public:
      inline explicit PID(const pid_exit_conditions &exitcondition, const pid_modifiables &modifiables) : _exitcondition(exitcondition), _modifiables(modifiables) {}
  };
}

#endif
