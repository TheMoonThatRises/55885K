/*
  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"
#include "pros/rtos.hpp"

#include <functional>

namespace KRONOS {
  class Robot : public DeviceManager, public ControllerManager {
    public:
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
        Gets device stored

        @param name Name of sign of the device

        @return The device requested
      */
      inline AbstractDevice* getDevice(const std::string &name) {
        return get(name);
      }

      /*
        Sets analog link for listener to listen 
        
        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const pros::controller_analog_e_t &method, std::function<void(int)> function, const controller_type &controller=master) {
        addLink(method, function, controller);
        
        return *this;
      }
      
      /*
        Sets digital link for listener to listen to

        @param method Controller analog input
        @param function Function to run
        @param controller Which controller input to read
      */
      inline Robot& addControllerLink(const pros::controller_digital_e_t &method, std::function<void()> function, const controller_type &controller=master) {
        addLink(method, function, controller);

        return *this;
      }

      /*
	      @param function
      */
      inline Robot &addControllerLink(std::function<void()> function) {
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
      inline void manipDevices(const std::vector<std::string> &dnames, std::function<void(std::pair<std::string, AbstractDevice*>)> manipFunc, int delay = 50) {
        for (std::pair<std::string, AbstractDevice*> device : valuesByKeys(dnames))
          manipFunc(device);

        pros::delay(delay);
      }
  };
}

#endif
