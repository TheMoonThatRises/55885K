/*
  Copyright 2025 Peter Duanmu

  @file base/managers/controllermanager.cpp

  Stores robot controls and functions
*/

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include "KRONOS/base/managers/controllermanager.hpp"

#include "KRONOS/assets/asserts.hpp"

namespace kronos::base::managers {
using assets::assert_not_nullptr;
using assets::Logger;
using assets::KRONOS_MSDELAY;

void ControllerManager::add(Controller *controller) {
  assert_not_nullptr(controller, "KRONOS::Controller");

  _controllers[controller->id()] = std::unique_ptr<Controller>(controller);
}

void ControllerManager::add(
  const pros::controller_analog_e_t& method,
  const single_analog_function& function,
  const assets::controller_type& controller) {
  (void) _analogLink.insert({{method, controller}, function});
}

void ControllerManager::add(
  const std::vector<pros::controller_analog_e_t>& methods,
  const multi_analog_function& function,
  const assets::controller_type& controller) {
  (void) _multiAnalogLink.insert({{methods, controller}, function});
}

void ControllerManager::add(
  const pros::controller_digital_e_t& method,
  const single_digital_function& function,
  const assets::controller_type& controller) {
  (void) _digitalLink.insert({{method, controller}, function});
}

void ControllerManager::add(
  const std::vector<pros::controller_digital_e_t>& method,
  const multi_digital_function& function,
  const assets::controller_type& controller) {
  (void) _multiDigitalLink.insert({{method, controller}, function});
}

void ControllerManager::add(const std::function<void()>& function) {
  _voidLinks.push_back(function);
}

void ControllerManager::initialise_all() {
  Logger::info("Attempting to load analog event");
  (void) _taskManager->add_task(
    _taskNames[C_ANALOG],
    pros::Task([&]() {
      while (true) {
        for (const auto& [key, function] : _analogLink) {
          Controller *controller = _controllers[key.second].get();

          assert_not_nullptr(controller, "KRONOS::Controller");

          function(controller->get_analog(key.first));
        }

        for (const auto& [key, function] : _multiAnalogLink) {
          Controller *controller = _controllers[key.second].get();

          assert_not_nullptr(controller, "KRONOS::Controller");

          std::vector<double> analogs(key.first.size());

          (void) std::transform(
            key.first.begin(),
            key.first.end(),
            analogs.begin(),
            [&](const pros::controller_analog_e_t& analog) {
              return controller->get_analog(analog);
            });

          function(analogs);
        }

        pros::delay(KRONOS_MSDELAY);
      }
    },
    TASK_PRIORITY_MAX,
    TASK_STACK_DEPTH_DEFAULT,
    _taskNames[C_ANALOG].c_str()));

  Logger::info("Attempting to load digital event");
  (void) _taskManager->add_task(
    _taskNames[C_DIGITAL],
    pros::Task([&]() {
      while (true) {
        for (const auto& [key, function] : _digitalLink) {
          Controller *controller = _controllers[key.second].get();

          assert_not_nullptr(controller, "KRONOS::Controller");

          function(controller->get_digital(key.first));
        }

        for (const auto& [key, function] : _multiDigitalLink) {
          Controller *controller = _controllers[key.second].get();

          assert_not_nullptr(controller, "KRONOS::Controller");

          std::vector<bool> digitals(key.first.size());

          (void) std::transform(
            key.first.begin(),
            key.first.end(),
            digitals.begin(),
            [&](const pros::controller_digital_e_t& digital) {
              return controller->get_digital(digital);
            });

          function(digitals);
        }

        pros::delay(KRONOS_MSDELAY);
      }
    },
    TASK_PRIORITY_MAX,
    TASK_STACK_DEPTH_DEFAULT,
    _taskNames[C_DIGITAL].c_str()));

  Logger::info("Attempting to load void event");
  (void) _taskManager->add_task(
    _taskNames[C_VOID],
    pros::Task([&]() {
      while (true) {
        for (const auto& function : _voidLinks) {
          function();
        }

        pros::delay(KRONOS_MSDELAY);
      }
    },
    TASK_PRIORITY_MAX,
    TASK_STACK_DEPTH_DEFAULT,
    _taskNames[C_VOID].c_str()));
}

void ControllerManager::event_deinitialize() {
  Logger::info("Attempting to unload analog event");
  (void) _taskManager->kill_task(_taskNames[C_ANALOG]);

  Logger::info("Attempting to unload digital event");
  (void) _taskManager->kill_task(_taskNames[C_DIGITAL]);

  Logger::info("Attempting to unload void event");
  (void) _taskManager->kill_task(_taskNames[C_VOID]);
}

ControllerManager::ControllerManager(TaskManager *taskManager) {
  assert(taskManager);

  _taskManager = taskManager;
}

ControllerManager::~ControllerManager() {
  event_deinitialize();
}

Controller* ControllerManager::get_controller(
  const assets::controller_type& type) {
  return _controllers[type].get();
}

bool ControllerManager::has_controller(const assets::controller_type& type) {
  return _controllers[type].get();
}
}  // namespace kronos::base::managers
