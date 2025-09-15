/*
  Copyright 2025 Peter Duanmu

  @file base/extenders/pid.cpp

  PID class for KRONOS
*/

#include "KRONOS/base/extenders/pid.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

#include "pros/rtos.hpp"

namespace kronos::base::extenders {
using assets::pid_consts;
using assets::consistency_consts;
using assets::pid_exit_conditions;

PID::PID(
  const pid_exit_conditions& exitcondition,
  const pid_consts& pidconsts,
  const consistency_consts& consistencyconsts)
: _exitcondition(exitcondition),
_pidconsts(pidconsts),
_consistencyconsts(consistencyconsts) {}

double PID::tick(const double& target, const double& current) {
  if (
    _exitcondition == pid_exit_conditions::P_TIME &&
    !_starttime.has_value()
  ) {
    _starttime = pros::millis();
    _previousTime = pros::millis();
  }

  const double error = target - current;

  _integral += error;

  if (error == 0 || error > target) {
    _integral = 0;
  }

  const double _derivative = error - _previousError;

  const double output =
    (_pidconsts.kP * error) +
    (_pidconsts.kI * _integral) +
    (_pidconsts.kD * _derivative);

  _previousError = error;
  _previousTime = pros::millis();

  if ((
        _exitcondition == pid_exit_conditions::P_ERROR &&
        fabs(target - output) <= _pidconsts.errormargin) ||
      (
        _exitcondition == pid_exit_conditions::P_TIME &&
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

void PID::add_consistency_value(const double& value) {
  _consistencyValues.push_back(value);

  if (_consistencyValues.size() > _consistencyconsts.maxvalues) {
    (void) _consistencyValues.erase(_consistencyValues.begin());
  }
}

bool PID::consistency(const double& compare) {
  const double result =
    std::reduce(
      _consistencyValues.begin(),
      _consistencyValues.end()) / _consistencyValues.size();

  return result >= compare - _consistencyconsts.errormargin &&
          result <= compare + _consistencyconsts.errormargin;
}

void PID::drop_consistency() {
  _consistencyValues.clear();
}

void PID::reset() {
  _starttime.reset();
  _previousError = 0;
  _integral = 0;
}

void PID::set_exit_condition(const pid_exit_conditions& exit) {
  _exitcondition = exit;
}

void PID::set_pid_consts(const pid_consts& pidconsts) {
  _pidconsts = pidconsts;
}

void PID::set_max_speed(const double& max) {
  _pidconsts.maxspeed = max;
}
}  // namespace kronos::base::extenders
