/*
  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "base/devices.hpp"
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
      VarManager *_varManager { nullptr };

      Button *_select { nullptr }, *_color { nullptr };
      Controller *_controller { nullptr };

      std::string _currentAuton;

      std::map<std::string, std::function<void()>> _autons {{"noauton", {}}};

      bool _canRunAuton = false;

      const std::vector<std::string> _events {"s_auton", "s_color"};

      std::array<std::unique_ptr<pros::Task>, 2> _selectors;

      /*
        Get value from auton map by index

        @param index Index of value

        @return Auton function
      */
      inline std::pair<std::string, std::function<void()>> autonByIndex(const int &index) {
        auto iter = _autons.begin();
        std::advance(iter, index);
        return {iter->first, iter->second};
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

        @param select Select button
        @param lock Lock button
        @param controller Main controller
      */
      inline void set_assets(KRONOS::Button* select, KRONOS::Button* color, KRONOS::Controller* controller) {
        _color = color;
        _select = select;
        _controller = controller;
      }

      /*
        Runs the selected autonomous code
      */
      inline void run() {
        unload_auton_threads();
        _varManager->global_get<std::function<void(std::string)>>("setsigtocolor")->operator()("aimcamera");

        if (_canRunAuton) {
          KLog::Log::info("Running auton '" + _currentAuton + "'");
          _controller->set_text("Rng auton '" + _currentAuton + "'");

          _autons.at(_currentAuton)();
        } else {
          KLog::Log::warn("Skipping auton...");

          if (!_controller) {
            _controller->set_text("Skipping auton...");
          }
        }
      }
    public:
      /*
        Variable manager. Should be robot's
      */
      inline explicit AutonomousManager(VarManager *varManager) : _varManager(varManager) {}

      /*
        Load auton selector threads
      */
      inline void load_auton_threads() {
        KLog::Log::info("Starting auton selection");

        if (_controller == nullptr) {
          KLog::Log::error("Controller not loaded. Aborting auton threads loader");
        } else if (!_selectors[S_AUTON] || !_selectors[S_COLOR]) {
          if (!_color) {
            KLog::Log::warn("Color assets not properly loaded");
            _controller->set_text("Clr slct ast nt prply ldd");
            _selectors[S_COLOR] = nullptr;
          } else if (!_selectors[S_COLOR]) {
            KLog::Log::info("Loading color selector");
            _selectors[S_COLOR] =
              std::make_unique<pros::Task>([&](){
                  while (true) {
                    if (_color->get_value()) {
                      KUtil::side_color newColor = *_varManager->global_get<int>("side") == KUtil::S_BLUE ? KUtil::S_RED : KUtil::S_BLUE;
                      _varManager->global_set<int>("side", newColor);
                      _controller->set_text("Color << " + std::string(newColor == KUtil::S_BLUE ? "BLUE" : "RED"));
                    }

                    pros::delay(200);
                  }
                }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, _events[S_COLOR].c_str()
              );
          } else {
            KLog::Log::warn("Color listener already initialised");
          }

          if (!_select) {
            KLog::Log::warn("Auton assets not properly loaded");
            _controller->set_text("Atn slct ast nt prply ldd");
            _selectors[S_AUTON] = nullptr;
          } else if (!_selectors[S_AUTON]) {
            KLog::Log::info("Loading auton selector");
            _selectors[S_AUTON] =
              std::make_unique<pros::Task>([&]() {
                  int index = 0;
                  _currentAuton = autonByIndex(index).first;

                  _controller->set_text("Auton << " + _currentAuton);
                  while (true) {
                    if (_select->get_value()) {
                      index = index == _autons.size() - 1 ? 0 : index + 1;

                      _currentAuton = autonByIndex(index).first;
                      _canRunAuton = index != 0;
                      _controller->set_text("Auton << " + _currentAuton);
                    }

                    pros::delay(200);
                  }
                }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, _events[S_AUTON].c_str()
              );
          } else {
            KLog::Log::warn("Auton listener already initialised");
          }
        } else {
          KLog::Log::warn("Auton and color listeners already initialised");
        }
      }

      inline void unload_auton_threads() {
        if (_selectors[S_COLOR]) {
          KLog::Log::info("Unloading color selector");
          _selectors[S_COLOR].get()->remove();
          _selectors[S_COLOR].reset(nullptr);
        }

        if (_selectors[S_AUTON]) {
          KLog::Log::info("Unloading auton selector");
          _selectors[S_AUTON].get()->remove();
          _selectors[S_AUTON].reset(nullptr);
        }
      }
  };
}

#endif
