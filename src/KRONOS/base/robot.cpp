/*
  Copyright 2025 Peter Duanmu

  @file base/robot.cpp

  Stores robot controls and functions
*/

#include <functional>
#include <string>
#include <vector>
#include <utility>

#include "KRONOS/base/robot.hpp"

namespace kronos::base {
using assets::Logger;
using assets::AbstractKronosError;
using assets::side_color;
using assets::pid_consts;
using assets::controller_type;

using managers::single_analog_function;
using managers::multi_analog_function;
using managers::single_digital_function;
using managers::multi_digital_function;

using user::to_motor;
using user::to_rotation;

Robot::Robot(
  bool use_memory_profiler,
  bool detailed_memory_profiler)
: AutonomousManager(this, this, &_htop),
  ControllerManager(this),
  SafetyManager(this, this, this),
  VarManager(_secret_key),
  _htop(_secret_key),
  _memory_profiler(this, detailed_memory_profiler) {
  if (use_memory_profiler) {
    _memory_profiler.enable_memory_profiler();
  }

  VarManager::global_set("side", side_color::S_RED, _htop.get_code());

  Logger::info("Constructing robot");
  Logger::info(
    "Current status: Autonomous: " +
    std::to_string(pros::competition::is_autonomous()) +
    " Connected: " + std::to_string(pros::competition::is_connected()) +
    " Disabled: " + std::to_string(pros::competition::is_disabled()));

  (void) std::set_terminate([]() {
    try {
      std::rethrow_exception(std::current_exception());
    } catch (const AbstractKronosError& kronosexcept) {
      Logger::error(
        "Uncaught KRONOS exception:" + std::string(kronosexcept.what()));
    } catch (const std::exception& exception) {
      Logger::error(
        "Uncaught fatal exception: " + std::string(exception.what()));
    }

    std::abort();
  });
}

Robot::~Robot() {
  Logger::info("Destructuring robot class");
}

Robot& Robot::add_device(const std::string& name, AbstractDevice *device) {
  DeviceManager::set(name, device);

  return *this;
}

Robot& Robot::add_device(Controller *controller) {
  ControllerManager::add(controller);

  return *this;
}

Robot& Robot::set_chassis_motors(const std::vector<Motor*>& motors) {
  ChassisManager::set_motors(motors);

  return *this;
}

Robot& Robot::set_chassis_motors(
  const std::vector<AbstractDevice*>& devices) {
  std::vector<Motor*> motors(devices.size());

  (void) std::transform(
    devices.begin(),
    devices.end(),
    motors.begin(),
    [&](AbstractDevice *device) {
      return to_motor(device);
    });

  ChassisManager::set_motors(motors);

  return *this;
}

Robot& Robot::set_chassis_odoms(const std::vector<Rotation*>& odoms) {
  ChassisManager::set_odoms(odoms);

  return *this;
}

Robot& Robot::set_chassis_odoms(
  const std::vector<AbstractDevice*>& devices) {
  std::vector<Rotation*> motors(devices.size());

  (void) std::transform(
    devices.begin(),
    devices.end(),
    motors.begin(),
    [&](AbstractDevice *device) {
      return to_rotation(device);
    });

  ChassisManager::set_odoms(motors);

  return *this;
}

Robot& Robot::set_chassis_pid(const pid_consts& gains) {
  ChassisManager::set_pid_consts(gains);

  return *this;
}

Robot& Robot::set_chassis_use_pid(const bool& use) {
  ChassisManager::use_pid(use);

  return *this;
}

Robot& Robot::set_auton_assets(Controller* controller) {
  AutonomousManager::set_assets(controller);

  return *this;
}

Robot& Robot::add_controller_link(
  const pros::controller_analog_e_t& method,
  const single_analog_function& function,
  const controller_type& controller) {
  ControllerManager::add(method, function, controller);

  return *this;
}

Robot& Robot::add_controller_link(
  const std::vector<pros::controller_analog_e_t>& methods,
  const multi_analog_function& function,
  const controller_type& controller) {
  ControllerManager::add(methods, function, controller);

  return *this;
}

Robot& Robot::add_controller_link(
  const pros::controller_digital_e_t& method,
  const single_digital_function& function,
  const controller_type& controller) {
  ControllerManager::add(method, function, controller);

  return *this;
}

Robot& Robot::add_controller_link(
  const std::vector<pros::controller_digital_e_t>& method,
  const multi_digital_function& function,
  const controller_type& controller) {
  ControllerManager::add(method, function, controller);

  return *this;
}

Robot& Robot::add_controller_link(const std::function<void()>& function) {
  ControllerManager::add(function);

  return *this;
}

Robot& Robot::add_auton(
  const std::string& name,
  std::function<void()> auton) {
  AutonomousManager::add(name, auton);

  return *this;
}

Robot& Robot::load_auton_threads() {
  (void) kill_all_tasks();
  AutonomousManager::load_auton();

  return *this;
}

Robot& Robot::run_auton() {
  (void) kill_all_tasks();
  AutonomousManager::run();

  return *this;
}

Robot& Robot::set_current_auton(const std::string& name) {
  (void) AutonomousManager::set_auton(name);

  return *this;
}

Robot& Robot::event_initialiser() {
  (void) kill_all_tasks();
  ControllerManager::initialise_all();

  return *this;
}

Robot& Robot::kill_all_tasks() {
  AutonomousManager::unload_auton_threads();
  ControllerManager::event_deinitialize();

  return *this;
}

void Robot::manipulate_devices(
  const std::vector<std::string>& dnames,
  const std::function<void(std::pair<std::string, AbstractDevice*>)>& func,
  int delay) {
  std::vector<std::pair<std::string, AbstractDevice *>> devices =
    DeviceManager::devices_by_key(dnames);

  for (const std::pair<std::string, AbstractDevice*>& device : devices) {
    func(device);
  }

  pros::delay(delay);
}

template<class T>
Robot& Robot::global_set(const std::string& key, const T& value) {
  VarManager::global_set(key, value);

  return *this;
}

void Robot::sleep(const size_t& milliseconds) {
  pros::delay(milliseconds);
}
}  // namespace kronos::base
