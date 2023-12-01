/*
  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "base/devices.hpp"
#include "base/managers/taskmanager.hpp"
#include "base/managers/varmanager.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace KRONOS {
  enum autonomous_events {
    S_AUTON, S_COLOR
  };

  class AutonomousManager {
    private:
      TaskManager *_taskManager;

      const std::string _taskName = "selector";

      inline static VarManager *_varManager { nullptr };

      inline static Controller *_controller { nullptr };

      inline static int _currentAutonIndex = 0;

      inline static std::string _currentAuton = "noauton";

      inline static std::map<std::string, std::function<void()>> _autons {{"noauton", {}}};

      /*
        Get value from auton map by index

        @param index Index of value

        @return Auton function
      */
      inline static std::pair<std::string, std::function<void()>> autonByIndex(const int &index) {
        auto iter = _autons.begin();
        std::advance(iter, index);
        return {iter->first, iter->second};
      }

      /*
        LVGL Auton button listener
      */
      inline static lv_res_t button_listener(lv_obj_t* btn) {
        uint8_t id = lv_obj_get_free_num(btn);

        switch (id) {
          case S_AUTON:
            _currentAutonIndex = _currentAutonIndex == _autons.size() - 1 ? 0 : _currentAutonIndex + 1;

            _currentAuton = autonByIndex(_currentAutonIndex).first;
            _controller->set_text("Auton << " + _currentAuton);
            break;
          case S_COLOR:
            KUtil::side_color newColor = *_varManager->global_get<int>("side") == KUtil::S_BLUE ? KUtil::S_RED : KUtil::S_BLUE;
            _varManager->global_set<int>("side", newColor);
            _controller->set_text("Color << " + std::string(newColor == KUtil::S_BLUE ? "BLUE" : "RED"));
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
      inline void add(const std::string &name, std::function<void()> auton) {
        _autons.insert({name, auton});
      }

      /*
        Sets peripheral select and lock button, and main controller for display

        @param controller Main controller
      */
      inline void set_assets(KRONOS::Controller* controller) {
        _controller = controller;
      }

      /*
        Runs the selected autonomous code
      */
      inline void run() {
        if (!_currentAuton.empty() && _currentAuton != "noauton") {
          KLog::Log::info("Running auton '" + _currentAuton + "'");
          _controller->set_text("Rng auton '" + _currentAuton + "'");

          _autons.at(_currentAuton)();
        } else {
          KLog::Log::warn("Skipping auton...");

          if (_controller) {
            _controller->set_text("Skipping auton...");
          }
        }
      }

      /*
        Load auton selector threads
      */
      inline void load_auton_threads() {
        if (!_taskManager->get_task(_taskName)) {
          KLog::Log::info("Starting auton selection");

          _taskManager->add_task(_taskName, pros::Task([&]() {
            _currentAuton = autonByIndex(_currentAutonIndex).first;

            _controller->set_text("Auton << " + _currentAuton);
            while (true) {
              lv_obj_clean(lv_scr_act());

              lv_obj_t* title = lv_label_create(lv_scr_act(), NULL);
              lv_label_set_text(title, "Auton buttons");
              lv_obj_align(title, nullptr, LV_ALIGN_IN_TOP_MID, 0, 5);

              lv_obj_t* autonbtn = lv_btn_create(lv_scr_act(), nullptr);
              lv_cont_set_fit(autonbtn, true, true); /*Enable resizing horizontally and vertically*/
              lv_obj_align(autonbtn, title, LV_ALIGN_IN_TOP_MID, 0, 10);
              lv_obj_set_free_num(autonbtn, 0);   /*Set a unique number for the button*/
              lv_btn_set_action(autonbtn, LV_BTN_ACTION_CLICK, button_listener);

              lv_obj_t* autonlabel = lv_label_create(autonbtn, nullptr);
              lv_label_set_text(autonlabel, _currentAuton.c_str());

              lv_obj_t* colorbtn = lv_btn_create(lv_scr_act(), nullptr);
              lv_cont_set_fit(colorbtn, true, true); /*Enable resizing horizontally and vertically*/
              lv_obj_align(colorbtn, title, LV_ALIGN_IN_TOP_MID, 0, 80);
              lv_obj_set_free_num(colorbtn, 1);   /*Set a unique number for the button*/
              lv_btn_set_action(colorbtn, LV_BTN_ACTION_CLICK, button_listener);

              lv_obj_t* colorlabel = lv_label_create(colorbtn, nullptr);
              lv_label_set_text(colorlabel, std::string(*_varManager->global_get<int>("side") == KUtil::S_BLUE ? "BLUE" : "RED").c_str());

              pros::delay(200);
            }
          }, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "autonselector"));
        } else {
          KLog::Log::warn("Auton selector already initialised");
        }
      }

      inline void unload_auton_threads() {
        KLog::Log::info("Unloading auton selector");
        _taskManager->kill_task(_taskName);
        lv_obj_clean(lv_scr_act());
      }
    public:
      /*
        Variable manager. Should be robot's
      */
      inline explicit AutonomousManager(VarManager *varManager, TaskManager *taskManager) {
        _varManager = varManager;
        _taskManager = taskManager;
      }

      inline ~AutonomousManager() {
        unload_auton_threads();
      }
  };
}

#endif
