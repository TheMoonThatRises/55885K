/*
  @file base/managers/controllermanager.hpp

  Stores robot controls and functions
*/

#ifndef _CONTROLLERMANAGER_HPP_
#define _CONTROLLERMANAGER_HPP_

#include "base/devices.hpp"

#include <functional>

namespace KRONOS {
  enum controller_type {
    master,
    partner
  };

  class ControllerManager {
    private:
      Controller *controllers[2];

      std::map<std::pair<pros::controller_analog_e_t, controller_type>, std::function<void(int)>> analogLink;
      std::map<std::pair<pros::controller_digital_e_t, controller_type>, std::function<void()>> digitalLink;
    protected:
      /*
        Set controller

        @param controller Controller as AbstractDevice pointer
      */
      inline void addController(Controller *controller) {
        if (controller->id() == pros::E_CONTROLLER_MASTER)
          controllers[master] = controller;
        else
          controllers[partner] = controller;
      }

      /*
        Sets analog link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void addLink(const pros::controller_analog_e_t &method, std::function<void(int)> function, const controller_type &controller=master) {
        analogLink.insert({{method, controller}, function});
      }
      
      /*
        Sets digital link for listener to listen to
        
        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void addLink(const pros::controller_digital_e_t &method, std::function<void()> function, const controller_type &controller=master) {
        digitalLink.insert({{method, controller}, function});
      }

      /*
        Listens to controller events
      */
      inline void listener() {
        for (const auto &[key, function] : analogLink)
          function(controllers[key.second]->get_analog(key.first));

        for (const auto &[key, function] : digitalLink)
          if (controllers[key.second]->get_digital(key.first))
            function();
      }
  };
}

#endif