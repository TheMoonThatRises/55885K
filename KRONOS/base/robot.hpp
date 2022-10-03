/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "base/managers/chassismanager.hpp"
#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"

#include "pros/rtos.hpp"

#include <functional>

namespace KRONOS {
  enum side_color {
    RED, GREEN
  };

  class Robot : public DeviceManager, public ControllerManager, public ChassisManager {
    protected:
      const side_color _color;
    public:
      inline explicit Robot(const side_color &color) : _color(color) {}

      /*
        Get color side the robot is on
      */
      inline side_color side() const { return _color; }

      /*
        Adds a device to the robot

        @param name Name to pair with the device
        @param device Device to set

        @return Self class
      */
      inline Robot& addDevice(const std::string &name, AbstractDevice *device) {
        set(name, device);

        return *this;
      }

      /*
        Adds a controller to the robot

        @param controller Controller to set

        @return Self class
      */
      inline Robot& addDevice(Controller *controller) {
        addController(controller);

        return *this;
      }

      /*
        Set chassis motors

        @param motors Vector of motor pointer
      */
      inline Robot& setChassisMotors(const std::vector<Motor*> &motors) {
        setChassis(motors);

        return *this;
      }

      /*
        Set chassis motors

        @param motors Vector of abstract device pointer
      */
      inline Robot& setChassisMotors(const std::vector<AbstractDevice*> &devices) {
        std::vector<Motor*> motors;

        for (AbstractDevice *device : devices)
          motors.push_back(dynamic_cast<Motor*>(device));

        setChassis(motors);

        return *this;
      }

      /*
        Sets analog link for listener to listen

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const pros::controller_analog_e_t &method, const std::function<void(double)>& function, const controller_type &controller=master) {
        addLink(method, function, controller);

        return *this;
      }

      /*
        Sets analog links for listener to listen to

        @param methods Multiple controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const std::vector<pros::controller_analog_e_t> &methods, const std::function<void(std::vector<double>)>& function, const controller_type &controller=master) {
        addLink(methods, function, controller);

        return *this;
      }

      /*
        Sets digital link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const pros::controller_digital_e_t &method, const std::function<void(bool)>& function, const controller_type &controller=master) {
        addLink(method, function, controller);

        return *this;
      }

      /*
        Sets digital links for listener to listen to

        @param method Controller analog inputs
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const std::vector<pros::controller_digital_e_t> &method, const std::function<void(std::vector<bool>)>& function, const controller_type &controller=master) {
        addLink(method, function, controller);

        return *this;
      }

      /*
	      @param function
      */
      inline Robot &addControllerLink(const std::function<void()>& function) {
        addLink(function);

        return *this;
      }

      /*
        Listens to controller events
      */
      inline Robot& controllerListener() {
        listener();

        return *this;
      }

      /*
        Manipulate groups of devices at the same time with the same command

        @param dnames Vector of names for devices
        @param manipFunc Function call that controls the device
        @param delay Delay after the manipDevices has ran
      */
      inline void manipDevices(const std::vector<std::string> &dnames, const std::function<void(std::pair<std::string, AbstractDevice*>)>& manipFunc, int delay = 50) {
        for (const std::pair<std::string, AbstractDevice*> &device : valuesByKeys(dnames))
          manipFunc(device);

        pros::delay(delay);
      }
  };
}

#endif
