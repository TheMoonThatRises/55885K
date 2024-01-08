/*
  Copyright 2024 Peter Duanmu

  @file base/managers/safetymanager.hpp

  Safety controller for the robot
*/

#ifndef KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_

#include <cassert>
#include <string>
#include <vector>

#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"
#include "base/managers/taskmanager.hpp"

#include "assets/statics.hpp"

namespace KRONOS {
class SafetyManager {
 private:
    ControllerManager *_controllerManager;
    DeviceManager *_deviceManager;
    TaskManager *_taskManager;

    const std::string _task_name = "safetymanager";

 protected:
    inline void _init() {
      _taskManager->add_task(
        _task_name,
        pros::Task([&]() {
          Controller *controller =
            this->_controllerManager->get_controller(
              this->_controllerManager->has_controller(C_PARTNER)
              ? C_PARTNER
              : C_MASTER);
          std::vector<Motor*> motor_list =
            this->_deviceManager->get_all_device_type<Motor>(K_MOTOR);

          while (true) {
            for (Motor *motor : motor_list) {
              if (motor->is_over_current()) {
                KLog::Log::warn(
                  "Motor at port '" + std::to_string(motor->port().value()) +
                  "' is over current");
                controller->set_text("Motor over current");
                controller->rumble("- . - .");
              } else if (motor->is_over_temp()) {
                KLog::Log::warn(
                  "Motor at port '" + std::to_string(motor->port().value()) +
                  "' is over temperature");
                controller->set_text("Motor over temp");
                controller->rumble("- - - -");
              }
            }

            pros::delay(KUtil::KRONOS_MSDELAY * 250);
          }
        },
        TASK_PRIORITY_MIN,
        TASK_STACK_DEPTH_MIN,
        _task_name.c_str()));
    }

 public:
    inline explicit SafetyManager(
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

    inline ~SafetyManager() {
      _taskManager->kill_task(_task_name);
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_
