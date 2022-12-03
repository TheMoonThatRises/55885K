/*
  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "base/devices.hpp"

#include <map>
#include <string>
#include <vector>

namespace KRONOS {
  class AutonomousManager {
    private:
      KRONOS::Button *_select { nullptr }, *_lock { nullptr };
      KRONOS::Controller *_controller { nullptr };

      std::string _currentAuton;

      std::map<std::string, std::vector<unsigned char>> _autons;

      bool _canRunAuton = false;
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
      inline void set_assets(KRONOS::Button &select, KRONOS::Button &lock, KRONOS::Controller &controller) {
        _select = &select;
        _lock = &lock;
        _controller = &controller;
      }

      /*
        Runs the selected autonomous code

        @param devices Vector of devices in devicemanager in robot
      */
      inline void run(std::vector<KRONOS::AbstractDevice*> devices) {
        if (_canRunAuton) {
          KLog::Log::info("Running auton '" + _currentAuton + "'");
        } else {
          KLog::Log::warn("Skipping auton...");

          if (_controller != nullptr) {
            _controller->set_text("Skipping auton...");
          }
        }
      }
    public:
      /*
        Runs the auton selector
      */
      inline void select_auton() {
        KLog::Log::info("Starting auton selection");

        if (_autons.find("noauton") == _autons.end()) {
          _autons.insert({ "noauton", {} });
        }

        if (_controller == nullptr) {
          KLog::Log::error("Controller not loaded");
        } else if (_select == nullptr || _lock == nullptr) {
          KLog::Log::error("Auton assets not properly loaded");
          _controller->set_text("Auton assets not properly loaded");
        } else {
          auto index = _autons.begin();
          _currentAuton = index->first;
          _controller->set_text("Selecting auton << " + _currentAuton);

          while (!_lock->get_value()) {
            if (_select->get_value()) {
              ++index;

              _currentAuton = index->first;
              _controller->set_text("Selecting auton << " + _currentAuton);
            }

            pros::delay(KRONOS_MSDELAY);
          }

          _controller->set_text("Locked auton < " + _currentAuton);
          _canRunAuton = true;
        }
      }
  };
}

#endif
