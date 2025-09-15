/*
  Copyright 2025 Peter Duanmu

  @file base/managers/safetymanager.hpp

  Safety controller for the robot
*/

#ifndef KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_

#include <string>

#include "KRONOS/base/managers/controllermanager.hpp"
#include "KRONOS/base/managers/devicemanager.hpp"
#include "KRONOS/base/managers/taskmanager.hpp"

namespace kronos::base::managers {
class SafetyManager {
 private:
    ControllerManager *_controllerManager;
    DeviceManager *_deviceManager;
    TaskManager *_taskManager;

    inline static const std::string _task_name = "safetymanager";

 protected:
    void _init();
    void unload_threads();

 public:
    explicit SafetyManager(
      ControllerManager *controllerManager,
      DeviceManager *deviceManager,
      TaskManager *taskManager);

    ~SafetyManager();
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_SAFETYMANAGER_HPP_
