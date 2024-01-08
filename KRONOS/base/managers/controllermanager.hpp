/*
  Copyright 2024 Peter Duanmu

  @file base/managers/controllermanager.hpp

  Stores robot controls and functions
*/

#ifndef KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "assets/asserts.hpp"

namespace KRONOS {

enum controller_type {
  C_MASTER, C_PARTNER
};

enum controller_events {
  C_ANALOG, C_DIGITAL, C_VOID
};

typedef std::pair<
  pros::controller_analog_e_t,
  controller_type> analog_single_id;
typedef std::pair<
  std::vector<pros::controller_analog_e_t>,
  controller_type> analog_multi_id;
typedef std::pair<
  pros::controller_digital_e_t,
  controller_type> digital_single_id;
typedef std::pair<
  std::vector<pros::controller_digital_e_t>,
  controller_type> digital_multi_id;

typedef std::function<void(double)> single_analog_function;
typedef std::function<void(std::vector<double>)> multi_analog_function;
typedef std::function<void(bool)> single_digital_function;
typedef std::function<void(std::vector<bool>)> multi_digital_function;

class ControllerManager {
 private:
    TaskManager *_taskManager;
    const std::array<std::string, 3> _taskNames {
      "c_analog",
      "c_digital",
      "c_void"};

    std::array<std::unique_ptr<Controller>, 2> _controllers;

    std::map<analog_single_id, single_analog_function> _analogLink;
    std::map<analog_multi_id, multi_analog_function> _multiAnalogLink;
    std::map<digital_single_id, single_digital_function> _digitalLink;
    std::map<digital_multi_id, multi_digital_function> _multiDigitalLink;
    std::vector<std::function<void()>> _voidLinks;

 protected:
    /*
      Set controller

      @param controller Controller as AbstractDevice pointer
    */
    inline void add(Controller *controller) {
      assert_not_nullptr(controller, "KRONOS::Controller");

      _controllers[controller->id()] = std::unique_ptr<Controller>(controller);
    }

    /*
      Sets analog link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    inline void add(
      const pros::controller_analog_e_t &method,
      const single_analog_function& function,
      const controller_type &controller = C_MASTER) {
      (void) _analogLink.insert({{method, controller}, function});
    }

    /*
      Sets analog links for listener to listen to

      @param methods Multiple controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    inline void add(
      const std::vector<pros::controller_analog_e_t> &methods,
      const multi_analog_function& function,
      const controller_type &controller = C_MASTER) {
      (void) _multiAnalogLink.insert({{methods, controller}, function});
    }

    /*
      Sets digital link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    inline void add(
      const pros::controller_digital_e_t &method,
      const single_digital_function& function,
      const controller_type &controller = C_MASTER) {
      (void) _digitalLink.insert({{method, controller}, function});
    }

    /*
      Sets digital links for listener to listen to

      @param method Controller analog inputs
      @param function Function to run
      @param controller Which controller input to read
    */
    inline void add(
      const std::vector<pros::controller_digital_e_t> &method,
      const multi_digital_function& function,
      const controller_type &controller = C_MASTER) {
      (void) _multiDigitalLink.insert({{method, controller}, function});
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
    inline void initialise_all() {
      if (!_taskManager->get_task(_taskNames[C_ANALOG]) ||
          !_taskManager->get_task(_taskNames[C_DIGITAL]) ||
          !_taskManager->get_task(_taskNames[C_VOID])) {
        _taskManager->add_task(
          _taskNames[C_ANALOG],
          pros::Task([&]() {
            while (true) {
              for (const auto &[key, function] : _analogLink) {
                KRONOS::Controller *controller = _controllers[key.second].get();

                assert_not_nullptr(controller, "KRONOS::Controller");

                function(controller->get_analog(key.first));
              }

              for (const auto &[key, function] : _multiAnalogLink) {
                KRONOS::Controller *controller = _controllers[key.second].get();

                assert_not_nullptr(controller, "KRONOS::Controller");

                std::vector<double> analogs(key.first.size());

                (void) std::transform(
                  key.first.begin(),
                  key.first.end(),
                  analogs.begin(),
                  [&](const pros::controller_analog_e_t &analog) {
                    return controller->get_analog(analog);
                  });

                function(analogs);
              }

              pros::delay(KUtil::KRONOS_MSDELAY);
            }
          },
          TASK_PRIORITY_MAX,
          TASK_STACK_DEPTH_DEFAULT,
          _taskNames[C_ANALOG].c_str()));

        _taskManager->add_task(
          _taskNames[C_DIGITAL],
          pros::Task([&]() {
            while (true) {
              for (const auto &[key, function] : _digitalLink) {
                KRONOS::Controller *controller = _controllers[key.second].get();

                assert_not_nullptr(controller, "KRONOS::Controller");

                function(controller->get_digital(key.first));
              }

              for (const auto &[key, function] : _multiDigitalLink) {
                KRONOS::Controller *controller = _controllers[key.second].get();

                assert_not_nullptr(controller, "KRONOS::Controller");

                std::vector<bool> digitals(key.first.size());

                (void) std::transform(
                  key.first.begin(),
                  key.first.end(),
                  digitals.begin(),
                  [&](const pros::controller_digital_e_t &digital) {
                    return controller->get_digital(digital);
                  });

                function(digitals);
              }

              pros::delay(KUtil::KRONOS_MSDELAY);
            }
          },
          TASK_PRIORITY_MAX,
          TASK_STACK_DEPTH_DEFAULT,
          _taskNames[C_DIGITAL].c_str()));

        _taskManager->add_task(
          _taskNames[C_VOID],
          pros::Task([&]() {
            while (true) {
              for (const auto &function : _voidLinks) {
                function();
              }

              pros::delay(KUtil::KRONOS_MSDELAY);
            }
          },
          TASK_PRIORITY_MAX,
          TASK_STACK_DEPTH_DEFAULT,
          _taskNames[C_VOID].c_str()));
      } else {
        KLog::Log::warn("Event listeners already initialised");
      }
    }

    inline void event_deinitialize() {
      assert(_taskManager->get_task(_taskNames[C_ANALOG]));
      KLog::Log::info("Unloading analog event");
      _taskManager->kill_task(_taskNames[C_ANALOG]);

      assert(_taskManager->get_task(_taskNames[C_DIGITAL]));
      KLog::Log::info("Unloading digital event");
      _taskManager->kill_task(_taskNames[C_DIGITAL]);

      assert(_taskManager->get_task(_taskNames[C_VOID]));
      KLog::Log::info("Unloading void event");
      _taskManager->kill_task(_taskNames[C_VOID]);
    }

 public:
    inline explicit ControllerManager(TaskManager *taskManager) {
      assert(taskManager);

      _taskManager = taskManager;
    }

    inline ~ControllerManager() {
      event_deinitialize();
    }

    /*
      Gets controller pointer stored

      @param type Controller type

      @return Controller pointer
    */
    inline Controller* get_controller(const controller_type &type) {
      return _controllers[type].get();
    }

    /*
      Queries if has specific controller

      @param type Controller type

      @returns If the controller manager contains the specific controller
    */
    inline bool has_controller(const controller_type &type) {
      return _controllers[type].get();
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_
