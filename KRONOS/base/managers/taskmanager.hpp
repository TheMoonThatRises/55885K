/*
  @file base/managers/taskmanager.hpp

  Task manager for KRONOS
*/

#ifndef _TASK_MANAGER_HPP_
#define _TASK_MANAGER_HPP_

#include "pros/rtos.hpp"

#include <map>

namespace KRONOS {
  class TaskManager {
    private:
      std::map<std::string, std::unique_ptr<pros::Task>> _tasks;
    public:
      inline ~TaskManager() {
        kill_all();
      }

      inline void add_task(const std::string &name, const pros::Task &task) {
        if (!get_task(name)) {
          _tasks.insert({ name, std::make_unique<pros::Task>(task) });
        }
      }

      inline pros::Task* get_task(const std::string& name) {
        return _tasks.find(name) != _tasks.end() ? _tasks.at(name).get() : nullptr;
      }

      inline void suspend_task(const std::string &name) {
        pros::Task *task = get_task(name);

        if (task) {
          task->suspend();
        }
      }

      inline void resume_task(const std::string &name) {
        pros::Task *task = get_task(name);

        if (task) {
          task->resume();
        }
      }

      inline void kill_task(const std::string &name) {
        if (get_task(name)) {
          _tasks.at(name)->suspend();
          _tasks.at(name)->remove();
          _tasks.at(name).reset(nullptr);
          _tasks.erase(name);
        }
      }

      inline void kill_all() {
        for (const auto &[name, task] : _tasks) {
          kill_task(name);
        }
      }
  };
}

#endif
