/*
  Copyright 2025 Peter Duanmu

  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_

#include <cassert>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "KRONOS/base/devices.hpp"
#include "KRONOS/base/managers/taskmanager.hpp"
#include "KRONOS/base/managers/varmanager.hpp"

#include "KRONOS/internal/hotp.hpp"

#include "liblvgl/lvgl.h"

namespace kronos::base::managers {
enum autonomous_events {
  S_AUTON, S_COLOR
};

class AutonomousManager {
 private:
    TaskManager *_taskManager;

    const std::string _taskName = "autonselector";

    inline static VarManager *_varManager { nullptr };

    inline static Controller *_controller { nullptr };

    inline static internal::HOTP *_hotp { nullptr };

    inline static size_t _currentAutonIndex = 0;

    inline static std::string _currentAuton = "noauton";

    inline static std::map<std::string, std::function<void()>> _autons {
      {"noauton", {}}
    };

    /*
      Get value from auton map by index

      @param index Index of value

      @return Auton function
    */
    static std::pair<std::string, std::function<void()>> autonByIndex(
      const size_t& index);

    /*
      LVGL Auton button listener
    */
    static void button_listener(lv_event_t* e);

 protected:
    /*
      Saves auton to auton map

      @param name Name of auton
      @param auton Auton vector
    */
    static void add(const std::string& name, std::function<void()> auton);

    /*
      Sets peripheral select and lock button, and main controller for display

      @param controller Main controller
    */
    static void set_assets(Controller* controller);

    /*
      Runs the selected autonomous code
    */
    static void run();

    /*
      Load auton selector threads
    */
    void load_auton();

    void unload_auton_threads();

    /*
      Select default auton
    */
    static void set_auton(const std::string& name);

 public:
    /*
      Variable manager. Should be robot's
    */
    explicit AutonomousManager(
      VarManager *varManager,
      TaskManager *taskManager,
      internal::HOTP *hotp);

    ~AutonomousManager();
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_
