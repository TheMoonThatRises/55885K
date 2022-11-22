/*
  @file base/managers/autonmanager.hpp

  Autonomous manager for KRONOS
*/

#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include <map>
#include <string>
#include <vector>

namespace KAuton {
  class Auton {
    private:
      std::string _currentAuton;

      std::map<std::string, std::vector<unsigned char>> _autons;
    protected:
      /*
        Saves auton to auton map
      */
      inline void add(const std::string &name, const std::vector<unsigned char> &auton) {
        _autons.insert({name, auton});
      }

      /*
        Runs the auton selector
      */
      inline void select();

      /*
        Runs the selected autonomous code
      */
      inline void run();
  };
}

#endif
