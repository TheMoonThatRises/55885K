/*
  @file base/managers/controllermanager.hpp

  Stores robot controls and functions
*/

#ifndef _CONTROLLERMANAGER_HPP_
#define _CONTROLLERMANAGER_HPP_

#include <functional>

namespace KRONOS {
  enum controller_type {
    C_MASTER, C_PARTNER
  };

  class ControllerManager {
    private:
      Controller *controllers[2];

      std::map<std::pair<pros::controller_analog_e_t, controller_type>, std::function<void(double)>> _analogLink;
      std::map<std::pair<std::vector<pros::controller_analog_e_t>, controller_type>, std::function<void(std::vector<double>)>> _multiAnalogLink;
      std::map<std::pair<pros::controller_digital_e_t, controller_type>, std::function<void(bool)>> _digitalLink;
      std::map<std::pair<std::vector<pros::controller_digital_e_t>, controller_type>, std::function<void(std::vector<bool>)>> _multiDigitalLink;
      std::vector<std::function<void()>> voidLinks;
    protected:
      /*
        Set controller

        @param controller Controller as AbstractDevice pointer
      */
      inline void add(Controller *controller) {
        if (controller->id() == pros::E_CONTROLLER_MASTER) {
          controllers[C_MASTER] = controller;
        } else {
          controllers[C_PARTNER] = controller;
        }
      }

      /*
        Sets analog link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void add(const pros::controller_analog_e_t &method, const std::function<void(double)>& function, const controller_type &controller=C_MASTER) {
        _analogLink.insert({{method, controller}, function});
      }

      /*
        Sets analog links for listener to listen to

        @param methods Multiple controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void add(const std::vector<pros::controller_analog_e_t> &methods, const std::function<void(std::vector<double>)>& function, const controller_type &controller=C_MASTER) {
        _multiAnalogLink.insert({{methods, controller}, function});
      }

      /*
        Sets digital link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void add(const pros::controller_digital_e_t &method, const std::function<void(bool)>& function, const controller_type &controller=C_MASTER) {
        _digitalLink.insert({{method, controller}, function});
      }

      /*
        Sets digital links for listener to listen to

        @param method Controller analog inputs
        @param function Function to run
        @param controller Which controller input to read
      */
      inline void add(const std::vector<pros::controller_digital_e_t> &method, const std::function<void(std::vector<bool>)>& function, const controller_type &controller=C_MASTER) {
        _multiDigitalLink.insert({{method, controller}, function});
      }

      /*
        Runs a function every loop in the listener

        @param function Function to run
      */
      inline void add(const std::function<void()>& function) {
        voidLinks.push_back(function);
      }

      /*
        Listens to controller and void events
      */
      inline void listener() {
        for (const auto &[key, function] : _analogLink)
          function(controllers[key.second]->get_analog(key.first));

        for (const auto &[key, function] : _multiAnalogLink) {
          std::vector<double> analogs;

          for (const pros::controller_analog_e_t &analog : key.first)
            analogs.push_back(controllers[key.second]->get_analog(analog));

          function(analogs);
        }

        for (const auto &[key, function] : _digitalLink)
          function(controllers[key.second]->get_digital(key.first));

        for (const auto &[key, function] : _multiDigitalLink) {
          std::vector<bool> digitals;

          for (const pros::controller_digital_e_t &digital : key.first)
            digitals.push_back(controllers[key.second]->get_digital(digital));

          function(digitals);
        }

        for (const auto &function : voidLinks)
          function();
      }
  };
}

#endif
