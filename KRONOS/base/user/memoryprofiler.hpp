/*
  @file base/user/memoryprofiler.hpp

  Tracks memory usage via FreeRTOS
*/

#ifndef _MEMORYPROFILER_HPP_
#define _MEMORYPROFILER_HPP_

#ifndef task_fn_t
  #define task_fn_t pros::task_fn_t
#endif

#include "base/managers/taskmanager.hpp"

#include "assets/logger.hpp"
#include "assets/statics.hpp"

// #include "pros/misc.h" // TODO: Add battery info
#include "pros/rtos.h"

#include "external/FreeRTOSConfig.h"
#include "external/portable.h"

namespace KMemoryProfiler {
  class MemoryProfiler {
    private:
      KRONOS::TaskManager *_taskManager;

      const size_t _min_ever_free_heap = xPortGetMinimumEverFreeHeapSize();
      const size_t _free_heap_min_buffer = 0x200;
      size_t _last_tick_free_heap;

      // const size_t _max_battery_capacity = pros::battery::

      const bool _detailed_tick_info;

      const std::string _task_name = "memoryprofiler";
    protected:
      inline void _init() {
        if (!_taskManager->get_task(_task_name)) {
          _taskManager->add_task(_task_name, pros::Task([&]() {
              while (true) {
                size_t free_heap_size = xPortGetFreeHeapSize();

                if (this->_detailed_tick_info) {
                  this->print_memory_info();
                } else {
                  if (free_heap_size - this->_free_heap_min_buffer <= this->_min_ever_free_heap) {
                    KLog::Log::warn("Low free heap left: " + std::to_string(free_heap_size));
                  }

                  KLog::Log::info("Memory used since last tick: " + std::to_string(free_heap_size - this->_last_tick_free_heap));
                }

                this->_last_tick_free_heap = free_heap_size;

                pros::delay(KUtil::KRONOS_MSDELAY * 500);
              }
            }, TASK_PRIORITY_MIN, TASK_STACK_DEPTH_MIN, _task_name.c_str()
          ));

          _taskManager->suspend_task(_task_name);
        } else {
          KLog::Log::warn("Memory profiler thread already initialized");
        }
      }
    public:
      inline explicit MemoryProfiler(KRONOS::TaskManager *taskManager, const bool &detailed_tick_info = false): _taskManager(taskManager), _last_tick_free_heap(xPortGetFreeHeapSize()), _detailed_tick_info(detailed_tick_info) {
        _init();
      }

      inline void print_memory_info() {
        bool loop_is_ticking = _taskManager->get_task(_task_name)->get_state() == pros::E_TASK_STATE_RUNNING;
        size_t free_heap_size = xPortGetFreeHeapSize();

        KLog::Log::info("|---------------[Memory Profiler]---------------|");
        KLog::Log::info("| Total Heap Size: " + std::to_string(configTOTAL_HEAP_SIZE));
        KLog::Log::info("| Minimum Free Heap Size: " + std::to_string(_min_ever_free_heap));
        KLog::Log::info("| Minimum Stack Size: " + std::to_string(configMINIMAL_STACK_SIZE));
        KLog::Log::info("| Current Heap Usage: " + std::to_string(free_heap_size));
        KLog::Log::info("| Last Tick Heap Usage: " + std::to_string(_last_tick_free_heap));
        KLog::Log::info("| Heap Usage Since Last Tick: " + std::to_string(free_heap_size - _last_tick_free_heap));

        if (!loop_is_ticking) {
          KLog::Log::warn("| Heap usage tick is not running");
        }

        if (free_heap_size - this->_free_heap_min_buffer <= this->_min_ever_free_heap) {
          KLog::Log::warn("| Low free heap left: " + std::to_string(free_heap_size));
        }

        KLog::Log::info("|-----------------------------------------------|");
      }

      inline void enable_memory_profiler() {
        _taskManager->resume_task(_task_name);
      }

      inline void disable_memory_profiler() {
        _taskManager->suspend_task(_task_name);
      }
  };
}

#endif
