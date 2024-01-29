/*
  Copyright 2024 Peter Duanmu

  @file base/managers/taskmanager.hpp

  Task manager for KRONOS
*/

#ifndef KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_

#include <map>
#include <memory>
#include <string>

#include "assets/asserts.hpp"

#include "pros/rtos.hpp"

namespace KRONOS {
class TaskManager {
 private:
    std::map<std::string, std::unique_ptr<pros::Task>> _tasks;

 public:
    inline ~TaskManager() {
      kill_all();
    }

    inline pros::Task* get_task(const std::string& name) {
      return _tasks.find(name) != _tasks.end()
        ? _tasks.at(name).get()
        : nullptr;
    }

    inline bool add_task(const std::string &name, const pros::Task &task) {
      if (!get_task(name)) {
        (void) _tasks.insert({ name, std::make_unique<pros::Task>(task) });

        return true;
      } else {
        return false;
      }
    }

    inline bool suspend_task(const std::string &name) {
      pros::Task *task = get_task(name);

      if (task) {
        task->suspend();

        return true;
      } else {
        return false;
      }
    }

    inline bool resume_task(const std::string &name) {
      pros::Task *task = get_task(name);

      if (task) {
        task->resume();

        return true;
      } else {
        return false;
      }
    }

    inline bool kill_task(const std::string &name) {
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

    inline void kill_all() {
      for (const auto &[name, task] : _tasks) {
        bool result = kill_task(name);
        (void) result;
        assert(result);
      }
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_TASKMANAGER_HPP_
