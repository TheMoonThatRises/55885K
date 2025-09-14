/*
  Copyright 2024 Peter Duanmu

  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "liblvgl/lvgl.h"

#include "assets/asserts.hpp"

#include "base/devices.hpp"
#include "base/managers/taskmanager.hpp"
#include "base/managers/varmanager.hpp"

#include "internal/hotp.hpp"

namespace KRONOS {
enum autonomous_events {
  S_AUTON, S_COLOR
};

class AutonomousManager {
 private:
    TaskManager *_taskManager;

    const std::string _taskName = "autonselector";

    inline static VarManager *_varManager { nullptr };

    inline static Controller *_controller { nullptr };

    inline static KOTP::HOTP *_hotp { nullptr };

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
    inline static std::pair<std::string, std::function<void()>> autonByIndex(
      const size_t &index
    ) {
      assert(index <= _autons.size());

      return *std::next(_autons.begin(), index);
    }

    /*
      LVGL Auton button listener
    */
    inline static void button_listener(lv_event_t* e) {
      assert_not_nullptr(_controller, "KRONOS::Controller");
      assert_not_nullptr(_varManager, "KRONOS::VarManager");
      assert_not_nullptr(_hotp, "KOTP::HOTP");

      int id = reinterpret_cast<intptr_t>(lv_event_get_user_data(e));

      switch (id) {
        case S_AUTON:
          _currentAutonIndex = (_currentAutonIndex + 1) % _autons.size();

          _currentAuton = autonByIndex(_currentAutonIndex).first;
          _controller->set_text("Auton << " + _currentAuton);
          break;
        case S_COLOR:
          auto *side = _varManager->global_get<KUtil::side_color>("side");

          assert(side);

          auto newColor = *side == KUtil::S_BLUE
              ? KUtil::S_RED
              : KUtil::S_BLUE;

          _varManager->global_set<KUtil::side_color>(
            "side",
            newColor,
            _hotp->get_code());

          auto color_text =
            std::string(newColor == KUtil::S_BLUE ? "BLUE" : "RED");

          _controller->set_text("Color << " + color_text);
          break;
      }
    }

 protected:
    /*
      Saves auton to auton map

      @param name Name of auton
      @param auton Auton vector
    */
    inline static void add(
      const std::string &name,
      std::function<void()> auton) {
      (void) _autons.insert({name, auton});
    }

    /*
      Sets peripheral select and lock button, and main controller for display

      @param controller Main controller
    */
    inline static void set_assets(KRONOS::Controller* controller) {
      assert_not_nullptr(controller, "KRONOS::Controller");

      _controller = controller;
    }

    /*
      Runs the selected autonomous code
    */
    inline static void run() {
      assert_not_nullptr(_controller, "KRONOS::Controller");

      if (!_currentAuton.empty() && _currentAuton != "noauton") {
        KLog::Log::info("Running auton '" + _currentAuton + "'");
        _controller->set_text("Rng auton '" + _currentAuton + "'");

        _autons.at(_currentAuton)();
      } else {
        KLog::Log::warn("Skipping auton...");

        _controller->set_text("Skipping auton...");
      }
    }

    /*
      Load auton selector threads
    */
    inline void load_auton() {
      assert_not_nullptr(_taskManager, "KRONOS::TaskManager");
      assert_not_nullptr(_controller, "KRONOS::Controller");

      if (!_taskManager->get_task(_taskName)) {
        KLog::Log::info("Starting auton selection");

        KLog::Log::info("Attempting to load auton event");
        (void) _taskManager->add_task(
          _taskName,
          pros::Task([&]() {
            _currentAuton = autonByIndex(_currentAutonIndex).first;

            _controller->set_text("Auton << " + _currentAuton);

            while (true) {
              lv_obj_clean(lv_screen_active());

              lv_obj_t* title = lv_label_create(lv_screen_active());
              lv_label_set_text(title, "Auton buttons");
              lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

              lv_obj_t* autonbtn = lv_button_create(lv_screen_active());
              // Enable resizing horizontally and vertically
              lv_obj_set_size(autonbtn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
              lv_obj_align(autonbtn, LV_ALIGN_TOP_MID, 0, 10);
              // Set a unique number for the button
              lv_obj_add_event_cb(autonbtn,
                button_listener,
                LV_EVENT_CLICKED,
                reinterpret_cast<void*>(S_AUTON));

              lv_obj_t* autonlabel = lv_label_create(autonbtn);
              lv_label_set_text(autonlabel, _currentAuton.c_str());

              lv_obj_t* colorbtn = lv_button_create(lv_screen_active());
              // Enable resizing horizontally and vertically
              lv_obj_set_size(colorbtn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
              lv_obj_align(colorbtn, LV_ALIGN_TOP_MID, 0, 80);
              // Set a unique number for the button
              lv_obj_add_event_cb(
                colorbtn,
                button_listener,
                LV_EVENT_CLICKED,
                reinterpret_cast<void*>(S_COLOR));

              // auto current_colour =
              //   _varManager->global_get<KUtil::side_color>("side");

              // assert(current_colour);

              // auto color_str = std::string(
              //   *current_colour == KUtil::S_BLUE
              //     ? "BLUE"
              //     : "RED");
              // lv_obj_t* colorlabel = lv_label_create(colorbtn, nullptr);
              // lv_label_set_text(colorlabel, color_str.c_str());

              pros::delay(200);
            }
          },
          TASK_PRIORITY_MAX,
          TASK_STACK_DEPTH_DEFAULT,
          _taskName.c_str()));
      } else {
        KLog::Log::warn("Auton selector already initialised");
      }
    }

    inline void unload_auton_threads() {
      KLog::Log::info("Attempting to unload auton selector");
      (void) _taskManager->kill_task(_taskName);
      lv_obj_clean(lv_screen_active());
    }

    /*
      Select default auton
    */
    inline static void set_auton(const std::string& name) {
      _currentAuton = name;
      _currentAutonIndex = std::distance(_autons.begin(), _autons.find(name));
    }

 public:
    /*
      Variable manager. Should be robot's
    */
    inline explicit AutonomousManager(
      VarManager *varManager,
      TaskManager *taskManager,
      KOTP::HOTP *hotp) {
      assert(varManager);
      assert(taskManager);
      assert(hotp);

      _varManager = varManager;
      _taskManager = taskManager;
      _hotp = hotp;
    }

    inline ~AutonomousManager() {
      unload_auton_threads();
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_AUTONMANAGER_HPP_
