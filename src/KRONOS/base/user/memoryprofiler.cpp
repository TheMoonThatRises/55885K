/*
  Copyright 2025 Peter Duanmu

  @file base/user/memoryprofiler.cpp

  Tracks memory usage via FreeRTOS
*/

#include <cassert>
#include <string>

#include "KRONOS/base/user/memoryprofiler.hpp"

#include "KRONOS/assets/logger.hpp"
#include "KRONOS/assets/statics.hpp"

namespace kronos::base::user {
using assets::Logger;
using assets::KRONOS_MSDELAY;

using managers::TaskManager;

void MemoryProfiler::_init() {
    Logger::info("Attempting to load memory profiler event");
    (void) _taskManager->add_task(
      _task_name,
      pros::Task([&]() {
        while (true) {
          size_t free_heap_size = xPortGetFreeHeapSize();

          if (this->_detailed_tick_info) {
            this->print_memory_info();
          } else {
            size_t free_heap_buffer =
              free_heap_size - this->_free_heap_min_buffer;
            if (free_heap_buffer <= this->_min_ever_free_heap) {
              Logger::warn(
                "Low free heap left: " + std::to_string(free_heap_size));
            }

            Logger::info(
              "Memory used since last tick: " +
              std::to_string(free_heap_size - this->_last_tick_free_heap));
          }

          this->_last_tick_free_heap = free_heap_size;

          pros::delay(KRONOS_MSDELAY * 500);
        }
      },
      TASK_PRIORITY_MIN,
      TASK_STACK_DEPTH_MIN,
      _task_name.c_str()));

  assert(_taskManager->get_task(_task_name));
  (void) _taskManager->suspend_task(_task_name);
}

void MemoryProfiler::unload_thread() {
  Logger::info("Attempting to unload memory profiler event");
  _taskManager->kill_task(_task_name);
}

MemoryProfiler::MemoryProfiler(
  TaskManager *taskManager,
  const bool& detailed_tick_info)
: _taskManager(taskManager),
  _last_tick_free_heap(xPortGetFreeHeapSize()),
  _detailed_tick_info(detailed_tick_info) {
  assert(_taskManager);

  _init();
}

MemoryProfiler::~MemoryProfiler() {
  unload_thread();
}

void MemoryProfiler::print_memory_info() {
  uint32_t task_state = _taskManager->get_task(_task_name)->get_state();
  bool loop_is_ticking = task_state == pros::E_TASK_STATE_RUNNING;
  size_t free_heap_size = xPortGetFreeHeapSize();
  size_t heap_last_tick = free_heap_size - _last_tick_free_heap;
  size_t free_heap_buffer =
    free_heap_size - this->_free_heap_min_buffer;

  Logger::info("|---------------[Memory Profiler]---------------|");
  Logger::info(
    "| Total Heap Size: " + std::to_string(configTOTAL_HEAP_SIZE));
  Logger::info(
    "| Minimum Free Heap Size: " + std::to_string(_min_ever_free_heap));
  Logger::info(
    "| Minimum Stack Size: " + std::to_string(configMINIMAL_STACK_SIZE));
  Logger::info(
    "| Current Heap Usage: " + std::to_string(free_heap_size));
  Logger::info(
    "| Last Tick Heap Usage: " + std::to_string(_last_tick_free_heap));
  Logger::info(
    "| Heap Usage Since Last Tick: " + std::to_string(heap_last_tick));

  if (!loop_is_ticking) {
    Logger::warn("| Heap usage tick is not running");
  }

  if (free_heap_buffer <= this->_min_ever_free_heap) {
    Logger::warn(
      "| Low free heap left: " + std::to_string(free_heap_size));
  }

  Logger::info("|-----------------------------------------------|");
}

void MemoryProfiler::enable_memory_profiler() {
  assert(_taskManager->get_task(_task_name));
  (void) _taskManager->resume_task(_task_name);
}

void MemoryProfiler::disable_memory_profiler() {
  assert(_taskManager->get_task(_task_name));
  (void) _taskManager->suspend_task(_task_name);
}
}  // namespace kronos::base::user
