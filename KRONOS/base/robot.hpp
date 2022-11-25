/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "base/managers/autonmanager.hpp"
#include "base/managers/chassismanager.hpp"
#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"

#include "pros/rtos.hpp"

#include <functional>

namespace KRONOS {
  enum side_color {
    RED, GREEN
  };

  class Robot : public DeviceManager, public ControllerManager, public ChassisManager, public AutonomousManager {
    protected:
      const side_color _color;
    public:
      /*
        @param color
      */
      inline explicit Robot(const side_color &color) : _color(color) {}

      /*
        Get color side the robot is on

        @return Which side the robot is on
      */
      inline side_color side() const { return _color; }

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
          motors.push_back(dynamic_cast<Motor*>(device));

        ChassisManager::set(motors);

        return *this;
      }

      /*
        Sets analog link for listener to listen

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read

        @return Reference to initial robot class
      */
      inline Robot& add_controller_link(const pros::controller_analog_e_t &method, const std::function<void(double)>& function, const controller_type &controller=master) {
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
      inline Robot& add_controller_link(const std::vector<pros::controller_analog_e_t> &methods, const std::function<void(std::vector<double>)>& function, const controller_type &controller=master) {
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
      inline Robot& add_controller_link(const pros::controller_digital_e_t &method, const std::function<void(bool)>& function, const controller_type &controller=master) {
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
      inline Robot& add_controller_link(const std::vector<pros::controller_digital_e_t> &method, const std::function<void(std::vector<bool>)>& function, const controller_type &controller=master) {
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
      inline Robot& add_auton(const std::string &name, const std::vector<unsigned char> &auton) {
        AutonomousManager::add(name, auton);

        return *this;
      }

      /*
        Runs selected auton
      */
      inline void run_auton() {
        AutonomousManager::run(DeviceManager::get_all());
      }

      /*
        Listens to controller events

        @return Reference to initial robot class
      */
      inline Robot& controller_listener() {
        ControllerManager::listener();

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
  };
}

#endif
