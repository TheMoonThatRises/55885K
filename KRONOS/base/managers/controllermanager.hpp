/*
  @file base/managers/controllermanager.hpp

  Stores robot controls and functions
*/

#ifndef _CONTROLLERMANAGER_HPP_
#define _CONTROLLERMANAGER_HPP_

#include <functional>
#include <memory>

namespace KRONOS {
  enum controller_type {
    C_MASTER, C_PARTNER
  };

  enum controller_events {
    C_ANALOG, C_DIGITAL, C_VOID
  };

  class ControllerManager {
    private:
      std::array<std::unique_ptr<Controller>, 2> _controllers;

      std::map<std::pair<pros::controller_analog_e_t, controller_type>, std::function<void(double)>> _analogLink;
      std::map<std::pair<std::vector<pros::controller_analog_e_t>, controller_type>, std::function<void(std::vector<double>)>> _multiAnalogLink;
      std::map<std::pair<pros::controller_digital_e_t, controller_type>, std::function<void(bool)>> _digitalLink;
      std::map<std::pair<std::vector<pros::controller_digital_e_t>, controller_type>, std::function<void(std::vector<bool>)>> _multiDigitalLink;
      std::vector<std::function<void()>> _voidLinks;

      const std::vector<std::string> _events {"c_analog", "c_digital", "c_void"};
      std::array<std::unique_ptr<pros::Task>, 3> _controllerListeners;
    protected:
      /*
        Set controller

        @param controller Controller as AbstractDevice pointer
      */
      inline void add(Controller *controller) {
        _controllers[controller->id()] = std::unique_ptr<Controller>(controller);
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
        _voidLinks.push_back(function);
      }

      /*
        Initialises all robot controller listening tasks
      */
      inline void event_initialiser() {
        if (!_controllerListeners[C_ANALOG] || !_controllerListeners[C_DIGITAL] || !_controllerListeners[C_VOID]) {
          _controllerListeners[C_ANALOG] =
            std::make_unique<pros::Task>([&]() {
              while (true) {
                for (const auto &[key, function] : _analogLink)
                  function(_controllers[key.second]->get_analog(key.first));

                for (const auto &[key, function] : _multiAnalogLink) {
                  std::vector<double> analogs;

                  for (const auto &analog : key.first)
                    analogs.push_back(_controllers[key.second]->get_analog(analog));

                  function(analogs);
                }

                pros::delay(KUtil::KRONOS_MSDELAY);
              }
            }, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, _events[C_ANALOG].c_str()
          );

          _controllerListeners[C_DIGITAL] =
            std::make_unique<pros::Task>([&]() {
                while (true) {
                  for (const auto &[key, function] : _digitalLink)
                    function(_controllers[key.second]->get_digital(key.first));

                  for (const auto &[key, function] : _multiDigitalLink) {
                    std::vector<bool> digitals;

                    for (const auto &digital : key.first)
                      digitals.push_back(_controllers[key.second]->get_digital(digital));

                    function(digitals);
                  }

                  pros::delay(KUtil::KRONOS_MSDELAY);
                }
              }, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, _events[C_DIGITAL].c_str()
            );

          _controllerListeners[C_VOID] =
            std::make_unique<pros::Task>([&]() {
              while (true) {
                for (const auto &function : _voidLinks) {
                  function();
                }

                pros::delay(KUtil::KRONOS_MSDELAY);
              }
            }, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, _events[C_VOID].c_str()
          );
        } else {
          KLog::Log::warn("Event listeners already initialised");
        }
      }
    public:
      /*
        Gets controller pointer stored

        @param type Controller type

        @return Controller pointer
      */
      inline Controller* get_controller(const controller_type &type) {
        return _controllers[type].get();
      }
  };
}

#endif
