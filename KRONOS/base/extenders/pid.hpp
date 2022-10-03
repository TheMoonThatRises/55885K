/*
  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

namespace KPID {
  enum pid_exit_conditions {
    error, time
  };

  class PID {
    protected:
      const pid_exit_conditions _exitcondition;
      const double errormargin = 0.1,
                   _kP = 0.5,
                   _kI = 0,
                   _kD = 0.4;
      double _previous;

      /*
        Get PID tuned value for target

        @param target Target position of the motor
      */
      inline double pid(const double &target, const double &current, const bool &init = false) {
        if (init) {
          _previous = 0;
        }

        const double output = (_kP * (target - current)) + (_kD * (_previous - current));

        if (_exitcondition == error && output <= errormargin) {
          return 0;
        } else {
          return output;
        }
      }
    public:
      inline explicit PID(const pid_exit_conditions &exitcondition) : _exitcondition(exitcondition) {}
  };
}
