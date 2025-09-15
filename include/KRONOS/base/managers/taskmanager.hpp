/*
  Copyright 2025 Peter Duanmu

  @file base/managers/taskmanager.hpp

  Task manager for KRONOS
*/

#ifndef KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_

#include <map>
#include <memory>
#include <string>

#include "pros/rtos.hpp"

namespace kronos::base::managers {
class TaskManager {
 private:
    std::map<std::string, std::unique_ptr<pros::Task>> _tasks;

 public:
    ~TaskManager();

    pros::Task* get_task(const std::string& name);
    bool add_task(const std::string& name, const pros::Task& task);
    bool suspend_task(const std::string& name);
    bool resume_task(const std::string& name);
    bool kill_task(const std::string& name);
    void kill_all();
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_
