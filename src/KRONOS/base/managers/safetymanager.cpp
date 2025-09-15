/*
  Copyright 2025 Peter Duanmu

  @file base/managers/safetymanager.cpp

  Safety controller for the robot
*/

#include <cassert>
#include <string>
#include <vector>

#include "KRONOS/base/managers/safetymanager.hpp"

#include "KRONOS/assets/asserts.hpp"
#include "KRONOS/assets/statics.hpp"

namespace kronos::base::managers {
using assets::assert_not_nullptr;
using assets::Logger;
using assets::KRONOS_MSDELAY;
using assets::device_types;
using assets::controller_type;

void SafetyManager::_init() {
  Logger::info("Attempting to load safety manager event");
  // (void) _taskManager->add_task(
  //   _task_name,
  //   pros::Task([&]() {
  //     Controller *controller =
  //       this->_controllerManager->get_controller(
  //         this->_controllerManager->has_controller(controller_type::C_PARTNER)
  //         ? controller_type::C_PARTNER
  //         : controller_type::C_MASTER);

  //     assert_not_nullptr(controller, "KRONOS::Controller");

  //     std::vector<Motor*> motor_list =
  //       this->_deviceManager->get_all_device_type<Motor>(device_types::K_MOTOR);

  //     while (true) {
  //       if (motor_list.size() < 1) {
  //         return;
  //       }

  //       for (Motor *motor : motor_list) {
  //         if (motor->is_over_current()) {
  //           Logger::warn(
  //             "Motor at port '" + std::to_string(motor->port().value()) +
  //             "' is over current");
  //           controller->set_text("Motor over current");
  //           controller->rumble("- . - .");
  //         } else if (motor->is_over_temp()) {
  //           Logger::warn(
  //             "Motor at port '" + std::to_string(motor->port().value()) +
  //             "' is over temperature");
  //           controller->set_text("Motor over temp");
  //           controller->rumble("- - - -");
  //         }
  //       }

  //       pros::delay(KRONOS_MSDELAY * 250);
  //     }
  //   },
  //   TASK_PRIORITY_MIN,
  //   TASK_STACK_DEPTH_MIN,
  //   _task_name.c_str()));
}

void SafetyManager::unload_threads() {
  Logger::info("Attempting to unload safety manager event");
  (void) _taskManager->kill_task(_task_name);
}

SafetyManager::SafetyManager(
  ControllerManager *controllerManager,
  DeviceManager *deviceManager,
  TaskManager *taskManager)
: _controllerManager(controllerManager),
  _deviceManager(deviceManager),
  _taskManager(taskManager) {
  #ifdef KRONOS_SAFETY_CHECKS
    assert(_controllerManager);
    assert(_deviceManager);
    assert(_taskManager);

    _init();
  #else
    this->~SafetyManager();
  #endif
}

SafetyManager::~SafetyManager() {
  unload_threads();
}
}  // namespace kronos::base::managers
