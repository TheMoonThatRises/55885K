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
      const int &index
    ) {
      assert(index >= 0);

      auto iter = _autons.begin();
      std::advance(iter, index);
      return {iter->first, iter->second};
    }

    /*
      LVGL Auton button listener
    */
    inline static lv_res_t button_listener(lv_obj_t* btn) {
      assert_not_nullptr(_controller, "KRONOS::Controller");
      assert_not_nullptr(_varManager, "KRONOS::VarManager");
      assert_not_nullptr(_hotp, "KOTP::HOTP");

      uint8_t id = lv_obj_get_free_num(btn);

      switch (id) {
        case S_AUTON:
          _currentAutonIndex = _currentAutonIndex == _autons.size() - 1
            ? 0
            : _currentAutonIndex + 1;

          _currentAuton = autonByIndex(_currentAutonIndex).first;
          _controller->set_text("Auton << " + _currentAuton);
          break;
        case S_COLOR:
          auto *side = _varManager->global_get<KUtil::side_color>("side");

          assert(side);

          auto newColor = *side == KUtil::S_BLUE
              ? KUtil::S_RED
              : KUtil::S_BLUE;

          _varManager->global_set<KUtil::side_color>("side", newColor, _hotp->next_code());

          auto color_text =
            std::string(newColor == KUtil::S_BLUE ? "BLUE" : "RED");

          _controller->set_text("Color << " + color_text);
          break;
      }

      return LV_RES_OK;
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
              lv_obj_clean(lv_scr_act());

              lv_obj_t* title = lv_label_create(lv_scr_act(), NULL);
              lv_label_set_text(title, "Auton buttons");
              lv_obj_align(title, nullptr, LV_ALIGN_IN_TOP_MID, 0, 5);

              lv_obj_t* autonbtn = lv_btn_create(lv_scr_act(), nullptr);
              // Enable resizing horizontally and vertically
              lv_cont_set_fit(autonbtn, true, true);
              lv_obj_align(autonbtn, title, LV_ALIGN_IN_TOP_MID, 0, 10);
              // Set a unique number for the button
              lv_obj_set_free_num(autonbtn, 0);
              lv_btn_set_action(autonbtn, LV_BTN_ACTION_CLICK, button_listener);

              lv_obj_t* autonlabel = lv_label_create(autonbtn, nullptr);
              lv_label_set_text(autonlabel, _currentAuton.c_str());

              lv_obj_t* colorbtn = lv_btn_create(lv_scr_act(), nullptr);
              // Enable resizing horizontally and vertically
              lv_cont_set_fit(colorbtn, true, true);
              lv_obj_align(colorbtn, title, LV_ALIGN_IN_TOP_MID, 0, 80);
              // Set a unique number for the button
              lv_obj_set_free_num(colorbtn, 1);
              lv_btn_set_action(colorbtn, LV_BTN_ACTION_CLICK, button_listener);

              auto *current_colour =
                _varManager->global_get<KUtil::side_color>("side");

              assert(current_colour);

              auto color_str = std::string(
                *current_colour == KUtil::S_BLUE
                  ? "BLUE"
                  : "RED");
              lv_obj_t* colorlabel = lv_label_create(colorbtn, nullptr);
              lv_label_set_text(colorlabel, color_str.c_str());

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
      lv_obj_clean(lv_scr_act());
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
