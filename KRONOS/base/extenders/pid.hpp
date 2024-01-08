/*
  Copyright 2024 Peter Duanmu

  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef KRONOS_BASE_EXTENDERS_PID_HPP_
#define KRONOS_BASE_EXTENDERS_PID_HPP_

#include <algorithm>
#include <cmath>
#include <numeric>
#include <optional>
#include <vector>

#include "pros/rtos.hpp"

namespace KExtender {
enum pid_exit_conditions {
  P_ERROR, P_NONE, P_TIME
};

struct pid_consts {
  double errormargin = 0.1,
         timeconstraint = 5000.0,
         minspeed = -600,
         maxspeed = 600,
         kP = 0.0,
         kI = 0.0,
         kD = 0.0;
};

struct consistency_consts {
  double errormargin = 10,
         maxvalues = 10;
};

class PID {
 private:
    std::optional<double> _starttime, _previousTime;
    double _previousError {}, _integral {};

    std::vector<double> _consistencyValues {};

    pid_exit_conditions _exitcondition;
    pid_consts _pidconsts;
    consistency_consts _consistencyconsts;

 public:
    /*
      @param exitcondition
      @param pidconsts
    */
    inline explicit PID(
      const pid_exit_conditions &exitcondition,
      const pid_consts &pidconsts,
      const consistency_consts &consistencyconsts)
    : _exitcondition(exitcondition),
      _pidconsts(pidconsts),
      _consistencyconsts(consistencyconsts) {}

    /*
      Get PID tuned value for target

      @param target Target position of the motor
      @param current Current position of the motor

      @return Value to set
    */
    inline virtual double tick(const double &target, const double &current) {
      if (_exitcondition == P_TIME && !_starttime.has_value()) {
        _starttime = pros::millis();
      }

      if (!_previousTime.has_value()) {
        _previousTime = pros::millis();
        return 0;
      }

      const double error = target - current;

      _integral += (error + _previousError) / 2;

      const double _derivative =
        (error - _previousError) / (pros::millis() - _previousTime.value());

      const double output =
        (_pidconsts.kP * error) +
        (_pidconsts.kI * _integral) +
        (_pidconsts.kD * _derivative);

      _previousError = error;
      _previousTime = pros::millis();

      if ((
            _exitcondition == P_ERROR &&
            fabs(target - output) <= _pidconsts.errormargin) ||
          (
            _exitcondition == P_TIME &&
            pros::millis() - _starttime.value() >= _pidconsts.timeconstraint)
      ) {
        reset();
        return 0;
      } else {
        return std::min(
          std::max(
            output,
            _pidconsts.minspeed),
          _pidconsts.maxspeed);
      }
    }

    inline virtual void add_consistency_value(const double &value) {
      _consistencyValues.push_back(value);

      if (_consistencyValues.size() > _consistencyconsts.maxvalues) {
        _consistencyValues.erase(_consistencyValues.begin());
      }
    }

    inline virtual bool consistency(const double &compare) {
      const double result =
        std::reduce(
          _consistencyValues.begin(),
          _consistencyValues.end()) / _consistencyValues.size();

      return result >= compare - _consistencyconsts.errormargin &&
             result <= compare + _consistencyconsts.errormargin;
    }

    inline virtual void drop_consistency() {
      _consistencyValues.clear();
    }

    inline virtual void reset() {
      _starttime.reset();
      _previousError = 0;
      _integral = 0;
    }

    inline virtual void set_exit_condition(const pid_exit_conditions &exit) {
      _exitcondition = exit;
    }

    inline virtual void set_pid_consts(const pid_consts &pidconsts) {
      _pidconsts = pidconsts;
    }

    inline virtual void set_max_speed(const double &max) {
      _pidconsts.maxspeed = max;
    }
};
}  // namespace KExtender

#endif  // KRONOS_BASE_EXTENDERS_PID_HPP_
