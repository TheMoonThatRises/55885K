/*
  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "base/devices.hpp"
#include "base/managers/varmanager.hpp"

#include <map>
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

      std::map<std::string, std::vector<unsigned char>> _autons;

      bool _canRunAuton = false;

      const std::vector<std::string> _events {"s_auton", "s_color"};

      std::array<std::unique_ptr<pros::Task>, 2> _selectors { nullptr, nullptr };
    protected:
      /*
        Saves auton to auton map

        @param name Name of auton
        @param auton Auton vector
      */
      inline void add(const std::string &name, const std::vector<unsigned char> &auton) {
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

        @param devices Vector of devices in devicemanager in robot
      */
      inline void run(const std::vector<KRONOS::AbstractDevice*> devices) {
        unload_auton_threads();

        if (_canRunAuton) {
          KLog::Log::info("Running auton '" + _currentAuton + "'");
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
      inline AutonomousManager(VarManager *varManager) : _varManager(varManager) {}

      /*
        Load auton selector threads
      */
      inline void load_auton_threads() {
        KLog::Log::info("Starting auton selection");

        if (_autons.find("noauton") == _autons.end()) {
          _autons.insert({ "noauton", {} });
        }

        if (_controller == nullptr) {
          KLog::Log::error("Controller not loaded. Aborting auton threads loader");
        } else if (!_selectors[S_AUTON] || !_selectors[S_COLOR]) {
          if (!_color) {
            KLog::Log::error("Color assets not properly loaded");
            _controller->set_text("Clr slct ast nt prply ldd");
            _selectors[S_COLOR] = nullptr;
          } else if (!_selectors[S_AUTON]) {
            KLog::Log::info("Loading color selector");
            _selectors[S_AUTON] =
              std::unique_ptr<pros::Task>(new pros::Task([&](){
                  while (true) {
                    if (_color->get_value()) {
                      KUtil::side_color newColor = *_varManager->global_get<int>("side") == KUtil::S_BLUE ? KUtil::S_RED : KUtil::S_BLUE;
                      _varManager->global_set<int>("side", newColor);
                      _controller->set_text("Color << " + std::string(newColor == KUtil::S_BLUE ? "BLUE" : "RED"));
                    }

                    pros::delay(500);
                  }
                }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, _events[S_COLOR].c_str())
              );
          } else {
            KLog::Log::warn("Color listener already initialised");
          }

          if (!_select) {
            KLog::Log::error("Auton assets not properly loaded");
            _controller->set_text("Atn slct ast nt prply ldd");
            _selectors[S_AUTON] = nullptr;
          } else if (!_selectors[S_AUTON]) {
            KLog::Log::info("Loading auton selector");
            _selectors[S_AUTON] =
              std::unique_ptr<pros::Task>(new pros::Task([&]() {
                  while (true) {
                    auto index = _autons.begin();
                    _currentAuton = index->first;
                    _controller->set_text("Selecting auton << " + _currentAuton);

                    if (_select->get_value()) {
                      ++index;

                      _currentAuton = index->first;
                      _canRunAuton = true;
                      _controller->set_text("Selecting auton << " + _currentAuton);
                    }

                    pros::delay(500);
                  }
                }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, _events[S_AUTON].c_str())
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
          _selectors[S_COLOR].reset();
        }

        if (_selectors[S_AUTON]) {
          KLog::Log::info("Unloading auton selector");
          _selectors[S_AUTON].get()->remove();
          _selectors[S_AUTON].reset();
        }
      }
  };
}

#endif
