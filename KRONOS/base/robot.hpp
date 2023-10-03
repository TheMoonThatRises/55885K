/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "assets/statics.hpp"

#include "base/managers/autonmanager.hpp"
#include "base/managers/chassismanager.hpp"
#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"
#include "base/managers/varmanager.hpp"
#include "base/managers/taskmanager.hpp"

#include "user/caster.hpp"
#include "user/memoryprofiler.hpp"

#include "pros/rtos.hpp"

#include <functional>

namespace KRONOS {
  class Robot : public AutonomousManager, public DeviceManager, public ChassisManager, public ControllerManager, public VarManager, public TaskManager {
    protected:
      KMemoryProfiler::MemoryProfiler _memory_profiler;
    public:
      inline explicit Robot() : AutonomousManager(this, this), ControllerManager(this), _memory_profiler(this) {
        KLog::Log::info("Constructing robot");
        KLog::Log::info("Current status: Autonomous: " + std::to_string(pros::competition::is_autonomous()) + " Connected: " + std::to_string(pros::competition::is_connected()) + " Disabled: " + std::to_string(pros::competition::is_disabled()));

        std::set_terminate([]() {
          try {
            std::rethrow_exception(std::current_exception());
          } catch (const AbstractKronosError &kronosexcept) {
            KLog::Log::error("Uncaught KRONOS exception:" + std::string(kronosexcept.what()));
          } catch (const std::exception &exception) {
            KLog::Log::error("Uncaught fatal exception: " + std::string(exception.what()));
          }

          std::abort();
        });
      }

      inline ~Robot() {
        KLog::Log::info("Destructuring robot class");
      }

      /*
        Adds a device to the robot

        @param name Name to pair with the device
        @param device Device to set

        @return Reference to initial robot class
      */
      inline Robot& add_device(const std::string &name, AbstractDevice *device) {
        DeviceManager::set(name, device);

        return *this;
      }

      /*
        Adds a controller to the robot

        @param controller Controller to set

        @return Reference to initial robot class
      */
      inline Robot& add_device(Controller *controller) {
        ControllerManager::add(controller);

        return *this;
      }

      /*
        Set chassis motors

        @param motors Vector of motor pointer

        @return Reference to initial robot class
      */
      inline Robot& set_chassis_motors(const std::vector<Motor*> &motors) {
        ChassisManager::set(motors);

        return *this;
      }

      /*
        Set chassis motors

        @param motors Vector of abstract device pointer

        @return Reference to initial robot class
      */
      inline Robot& set_chassis_motors(const std::vector<AbstractDevice*> &devices) {
        std::vector<Motor*> motors;

        for (AbstractDevice *device : devices)
          motors.push_back(to_motor(device));

        ChassisManager::set(motors);

        return *this;
      }

      /*
        Sets the devices that autonomous uses

        @param select Select button
        @param lock Lock button
        @param controller Main controller
      */
      inline Robot& set_auton_assets(KRONOS::Controller* controller) {
        AutonomousManager::set_assets(controller);

        return *this;
      }

      /*
        Sets analog link for listener to listen

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const pros::controller_analog_e_t &method, const std::function<void(double)>& function, const controller_type &controller=C_MASTER) {
        ControllerManager::add(method, function, controller);

        return *this;
      }

      /*
        Sets analog links for listener to listen to

        @param methods Multiple controller analog input
        @param function Function to run
        @param controller Which controller input to read

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const std::vector<pros::controller_analog_e_t> &methods, const std::function<void(std::vector<double>)>& function, const controller_type &controller=C_MASTER) {
        ControllerManager::add(methods, function, controller);

        return *this;
      }

      /*
        Sets digital link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const pros::controller_digital_e_t &method, const std::function<void(bool)>& function, const controller_type &controller=C_MASTER) {
        ControllerManager::add(method, function, controller);

        return *this;
      }

      /*
        Sets digital links for listener to listen to

        @param method Controller analog inputs
        @param function Function to run
        @param controller Which controller input to read

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const std::vector<pros::controller_digital_e_t> &method, const std::function<void(std::vector<bool>)>& function, const controller_type &controller=C_MASTER) {
        ControllerManager::add(method, function, controller);

        return *this;
      }

      /*
        Runs a function every loop of the listener

        @param function Function to run

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const std::function<void()>& function) {
        ControllerManager::add(function);

        return *this;
      }

      /*
        Saves auton to auton map

        @param name Name of auton
        @param auton Auton vector

        @return Reference to initial robot class
      */
      inline Robot& add_auton(const std::string &name, std::function<void()> auton) {
        AutonomousManager::add(name, auton);

        return *this;
      }

      inline Robot& load_auton_threads() {
        kill_all_tasks();
        AutonomousManager::load_auton_threads();

        return *this;
      }

      /*
        Runs selected auton
      */
      inline Robot& run_auton() {
        kill_all_tasks();
        AutonomousManager::run();

        return *this;
      }

      /*
        Listens to controller events

        @return Reference to initial robot class
      */
      inline Robot& event_initialiser() {
        kill_all_tasks();
        ControllerManager::event_initialiser();

        return *this;
      }

      inline Robot& kill_all_tasks() {
        AutonomousManager::unload_auton_threads();
        ControllerManager::event_deinitialize();
        TaskManager::kill_all();

        return *this;
      }

      /*
        Manipulate groups of devices at the same time with the same command

        @param dnames Vector of names for devices
        @param manip_func Function call that controls the device
        @param delay Delay after the manipDevices has ran
      */
      inline void manipulate_devices(const std::vector<std::string> &dnames, const std::function<void(std::pair<std::string, AbstractDevice*>)>& manip_func, int delay = 50) {
        for (const std::pair<std::string, AbstractDevice*> &device : DeviceManager::devices_by_key(dnames))
          manip_func(device);

        pros::delay(delay);
      }

      template<class T>
      inline Robot& global_set(const std::string &key, const T& value) {
        VarManager::global_set(key, value);

        return *this;
      }

      /*
        Delay's the robot's functionalities (Wrapper of pros::delay)

        @param milliseconds Length of sleep
      */
      inline void sleep(const int &milliseconds) {
        pros::delay(milliseconds);
      }
  };
}

#endif
