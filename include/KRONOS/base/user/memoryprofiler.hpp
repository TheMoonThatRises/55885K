/*
  Copyright 2025 Peter Duanmu

  @file base/user/memoryprofiler.hpp

  Tracks memory usage via FreeRTOS
*/

#ifndef KRONOS_BASE_USER_MEMORYPROFILER_HPP_
#define KRONOS_BASE_USER_MEMORYPROFILER_HPP_

#include <string>

#include "KRONOS/base/managers/taskmanager.hpp"

// #include "pros/misc.h" // TODO: Add battery info
#include "pros/rtos.h"

#ifndef task_fn_t
  #define task_fn_t pros::task_fn_t
#endif

#include "KRONOS/external/FreeRTOS/FreeRTOSConfig.h"
#include "KRONOS/external/FreeRTOS/portable.h"

namespace kronos::base::user {
class MemoryProfiler {
 private:
    managers::TaskManager *_taskManager;

    const size_t _min_ever_free_heap = xPortGetMinimumEverFreeHeapSize();
    const size_t _free_heap_min_buffer = 0x200;
    size_t _last_tick_free_heap;

    // const size_t _max_battery_capacity = pros::battery::

    const bool _detailed_tick_info;

    const std::string _task_name = "memoryprofiler";

 protected:
    void _init();
    void unload_thread();

 public:
    explicit MemoryProfiler(
      managers::TaskManager *taskManager,
      const bool& detailed_tick_info = false);

    ~MemoryProfiler();

    void print_memory_info();
    void enable_memory_profiler();
    void disable_memory_profiler();
};
}  // namespace kronos::base::user

#endif  // KRONOS_BASE_USER_MEMORYPROFILER_HPP_
