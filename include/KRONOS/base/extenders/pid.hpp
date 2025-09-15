/*
  Copyright 2025 Peter Duanmu

  @file base/extenders/pid.hpp

  PID class for KRONOS
*/

#ifndef KRONOS_BASE_EXTENDERS_PID_HPP_
#define KRONOS_BASE_EXTENDERS_PID_HPP_

#include <optional>
#include <vector>

#include "KRONOS/assets/structs.hpp"

namespace kronos::base::extenders {
class PID {
 private:
    std::optional<double> _starttime, _previousTime;
    double _previousError {}, _integral {};

    std::vector<double> _consistencyValues {};

    assets::pid_exit_conditions _exitcondition;
    assets::pid_consts _pidconsts;
    assets::consistency_consts _consistencyconsts;

 public:
    /*
      @param exitcondition
      @param pidconsts
    */
    explicit PID(
      const assets::pid_exit_conditions& exitcondition,
      const assets::pid_consts& pidconsts,
      const assets::consistency_consts& consistencyconsts);

    /*
      Get PID tuned value for target

      @param target Target position of the motor
      @param current Current position of the motor

      @return Value to set
    */
    virtual double tick(const double& target, const double& current);
    virtual void add_consistency_value(const double& value);
    virtual bool consistency(const double& compare);
    virtual void drop_consistency();
    virtual void reset();
    virtual void set_exit_condition(const assets::pid_exit_conditions& exit);
    virtual void set_pid_consts(const assets::pid_consts& pidconsts);
    virtual void set_max_speed(const double& max);
};
}  // namespace kronos::base::extenders

#endif  // KRONOS_BASE_EXTENDERS_PID_HPP_
