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
      std::map<std::string, std::unique_ptr<pros::Task>> tasks;
    public:
      void add_task(const std::string &name, const pros::Task &task) {
        tasks.insert({ name, std::make_unique<pros::Task>(task) });
      }

      pros::Task* get_task(const std::string& name) {
        return tasks.find(name) != tasks.end() ? tasks.at(name).get() : nullptr;
      }

      void suspend_task(const std::string &name) {
        if (tasks.find(name) != tasks.end()) {
          get_task(name)->suspend();
        }
      }

      void resume_task(const std::string &name) {
        if (tasks.find(name) != tasks.end()) {
          get_task(name)->resume();
        }
      }

      void kill_task(const std::string &name) {
        if (tasks.find(name) != tasks.end()) {
          tasks.at(name)->remove();
          tasks.at(name).reset(nullptr);
          tasks.erase(name);
        }
      }

      void kill_all() {
        for (const auto &[name, task] : tasks) {
          kill_task(name);
        }
      }
  };
}

#endif