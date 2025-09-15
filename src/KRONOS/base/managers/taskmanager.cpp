/*
  Copyright 2025 Peter Duanmu

  @file base/managers/taskmanager.cpp

  Task manager for KRONOS
*/

#include <cassert>
#include <memory>
#include <string>

#include "KRONOS/base/managers/taskmanager.hpp"

#include "KRONOS/assets/asserts.hpp"

namespace kronos::base::managers {
TaskManager::~TaskManager() {
  kill_all();
}

pros::Task* TaskManager::get_task(const std::string& name) {
  return _tasks.find(name) != _tasks.end()
    ? _tasks.at(name).get()
    : nullptr;
}

bool TaskManager::add_task(const std::string& name, const pros::Task& task) {
  if (!get_task(name)) {
    (void) _tasks.insert({ name, std::make_unique<pros::Task>(task) });

    return true;
  } else {
    return false;
  }
}

bool TaskManager::suspend_task(const std::string& name) {
  pros::Task *task = get_task(name);

  if (task) {
    task->suspend();

    return true;
  } else {
    return false;
  }
}

bool TaskManager::resume_task(const std::string& name) {
  pros::Task *task = get_task(name);

  if (task) {
    task->resume();

    return true;
  } else {
    return false;
  }
}

bool TaskManager::kill_task(const std::string& name) {
  pros::Task *task = get_task(name);

  if (task) {
    task->suspend();
    task->remove();
    _tasks.at(name).reset(nullptr);
    (void) _tasks.erase(name);

    return true;
  } else {
    return false;
  }
}

void TaskManager::kill_all() {
  for (const auto& [name, task] : _tasks) {
    bool result = kill_task(name);
    (void) result;
    assert(result);
  }
}
}  // namespace kronos::base::managers
