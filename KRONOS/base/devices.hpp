/*
  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef _DEVICES_HPP_
#define _DEVICES_HPP_

#include <iostream>
#include <map>

#include "assets/devicestructs.hpp"

#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/vision.hpp"

namespace KRONOS {
  enum device_types {
    BUTTON,
    CONTROLLER,
    MOTOR,
    PISTON,
    PROXIMITY,
    VISION
  };

  class AbstractDevice {
    protected:
      device_types _type;
    public:
      /*
        @param device
      */
      inline explicit AbstractDevice(const device_types &device) : _type(device) {};

      /*
        Gets the enum type of the class
      */
      inline virtual device_types classname() const { return _type; }
  };

  class Button : public pros::ADIDigitalIn, public AbstractDevice {
    public:
      inline explicit Button(const AbstractDeviceStruct &device) : pros::ADIDigitalIn(device.port), AbstractDevice(BUTTON) {};
  };

  class Controller : public pros::Controller, public AbstractDevice {
    protected:
      pros::controller_id_e_t _id;
    public:
      /*
        @param id Master or partner controller
      */
      inline explicit Controller(const ControllerStruct &controller) : pros::Controller(controller.id), AbstractDevice(CONTROLLER), _id(controller.id) {};

      /*
        Get controller id
      */
      inline pros::controller_id_e_t id() const { return _id; }

      /*
        Sets the controllers display screen text

        @param text
      */
      inline void setControllerText(const std::string &text) {
        clear();
        set_text(0, 0, text + "         ");

        std::cout << text << std::endl;
      }
  };

  class Motor : public pros::Motor, public AbstractDevice {
    public:
      /*
        @param port
      */
      inline explicit Motor(const MotorStruct &device) : pros::Motor(device.port, device.gearset, device.reverse, device.encoder), AbstractDevice(MOTOR) {
        set_brake_mode(device.brakemode);
      };
  };

  class Piston : public pros::ADIDigitalOut, public AbstractDevice {
    protected:
      bool _value;
    public:
      /*
        @param port
      */
      inline explicit Piston(const AbstractDeviceStruct &device) : pros::ADIDigitalOut(device.port), AbstractDevice(PISTON), _value(false) {};

      /*
        @param setValue
      */
      inline bool set_value(const bool &setValue) {
        pros::ADIDigitalOut::set_value(setValue);
        _value = setValue;

        return _value;
      }
      inline bool toggle() { return set_value(!_value); }
      inline bool value() const { return _value; }
  };

  class Proximity : public pros::Distance, public AbstractDevice {
    public:
      /*
        @param port
      */
      inline explicit Proximity(const AbstractDeviceStruct &device) : pros::Distance(device.port), AbstractDevice(PROXIMITY) {};
  };

  class Vision : pros::Vision, public AbstractDevice {
    protected:
      std::map<int, pros::vision_signature_s_t> _signatures;
    public:
      /*
        @param port
      */
      inline explicit Vision(const AbstractDeviceStruct &device) : pros::Vision(device.port), AbstractDevice(VISION) {};

      /*
        Save a vision signature to the vision sensor.

        @param name
        @param signature
      */
      inline void addSignature(const int &name, const pros::vision_signature_s_t &signature) { _signatures.insert({name, signature}); }

      /*
        Get a vision signature saved from the sensor.

        @param name
      */
      inline pros::vision_signature_s_t &getSignature(const int &name) { return _signatures.at(name); }

      /*
        Set the signature for the vision sensor.

        @param name
      */
      inline void setSignature(const int &name) { set_signature(name, &(getSignature(name))); }
  };
}

#endif
