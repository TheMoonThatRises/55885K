/*
  Copyright 2025 Peter Duanmu

  @file base/managers/controllermanager.hpp

  Stores robot controls and functions
*/

#ifndef KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "KRONOS/base/managers/taskmanager.hpp"
#include "KRONOS/base/devices.hpp"

#include "pros/misc.h"

namespace kronos::base::managers {
enum controller_events {
  C_ANALOG, C_DIGITAL, C_VOID
};

typedef std::pair<
pros::controller_analog_e_t,
assets::controller_type> analog_single_id;
typedef std::pair<
std::vector<pros::controller_analog_e_t>,
assets::controller_type> analog_multi_id;
typedef std::pair<
pros::controller_digital_e_t,
assets::controller_type> digital_single_id;
typedef std::pair<
std::vector<pros::controller_digital_e_t>,
assets::controller_type> digital_multi_id;

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
    void add(Controller *controller);

    /*
      Sets analog link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    void add(
      const pros::controller_analog_e_t& method,
      const single_analog_function& function,
      const assets::controller_type& controller
        = assets::controller_type::C_MASTER);

    /*
      Sets analog links for listener to listen to

      @param methods Multiple controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    void add(
      const std::vector<pros::controller_analog_e_t>& methods,
      const multi_analog_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Sets digital link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read
    */
    void add(
      const pros::controller_digital_e_t& method,
      const single_digital_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Sets digital links for listener to listen to

      @param method Controller analog inputs
      @param function Function to run
      @param controller Which controller input to read
    */
    void add(
      const std::vector<pros::controller_digital_e_t>& method,
      const multi_digital_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Runs a function every loop in the listener

      @param function Function to run
    */
    void add(const std::function<void()>& function);

    /*
      Initialises all robot controller listening tasks
    */
    void initialise_all();

    void event_deinitialize();

 public:
    explicit ControllerManager(TaskManager *taskManager);

    ~ControllerManager();

    /*
      Gets controller pointer stored

      @param type Controller type

      @return Controller pointer
    */
    Controller* get_controller(const assets::controller_type& type);

    /*
      Queries if has specific controller

      @param type Controller type

      @returns If the controller manager contains the specific controller
    */
    bool has_controller(const assets::controller_type& type);
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_CONTROLLERMANAGER_HPP_
