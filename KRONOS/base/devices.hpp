/*
  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef _DEVICES_HPP_
#define _DEVICES_HPP_

#include <iostream>
#include <map>

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

  class IOKParent {
    protected:
      device_types _type;
    public:
      /*
        @param device
      */
      inline explicit IOKParent(const device_types &device) : _type(device) {};

      /*
        Gets the enum type of the class
      */
      inline device_types classname() { return _type; }
  };

  class Button : public pros::ADIDigitalIn, public IOKParent {
    public:
      inline explicit Button(const int8_t &port) : pros::ADIDigitalIn(port), IOKParent(BUTTON) {};
  };

  class Controller : public pros::Controller, public IOKParent {
    protected:
      pros::controller_id_e_t _id;
    public:
      /*
        @param id Master or partner controller
      */
      inline explicit Controller(const pros::controller_id_e_t &id = pros::E_CONTROLLER_MASTER) : pros::Controller(id), IOKParent(CONTROLLER), _id(id) {};

      /*
        Get controller id
      */
      inline pros::controller_id_e_t id() { return _id; }

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

  class Motor : public pros::Motor, public IOKParent {
    public:
      /*
        @param port
      */
      inline explicit Motor(const int8_t &port, const pros::motor_gearset_e_t &gearset, const bool &reverse, const pros::motor_encoder_units_e_t &encoder, const pros::motor_brake_mode_e_t &brakemode) : pros::Motor(port, gearset, reverse, encoder), IOKParent(MOTOR) {
        set_brake_mode(brakemode);
      };
  };

  class Piston : public pros::ADIDigitalOut, public IOKParent {
    protected:
      bool _value;
    public:
      /*
        @param port
      */
      inline explicit Piston(const char &port) : pros::ADIDigitalOut(port), IOKParent(PISTON), _value(false) {};

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

  class Proximity : public pros::Distance, public IOKParent {
    public:
      /*
        @param port
      */
      inline explicit Proximity(const int8_t &port) : pros::Distance(port), IOKParent(PROXIMITY) {};
  };

  class Vision : pros::Vision, public IOKParent {
    protected:
      std::map<int, pros::vision_signature_s_t> _signatures;
    public:
      /*
        @param port
      */
      inline explicit Vision(const int8_t &port) : pros::Vision(port), IOKParent(VISION) {};

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
