/*
  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef _DEVICES_HPP_
#define _DEVICES_HPP_

#include "assets/devicestructs.hpp"
#include "assets/errortypes.hpp"
#include "assets/logger.hpp"
#include "assets/util.hpp"

#include "base/extenders/pid.hpp"

#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"

#include <map>

namespace KRONOS {
  class AbstractDevice {
    protected:
      const device_types _type;
      const device_face _face;
      const char _port;
    public:
      /*
        @param device
        @param delay
      */
      inline explicit AbstractDevice(const device_types &device, const device_face &face, const char &port) : _type(device), _face(face), _port(port) {
        if (_port == '\0') {
          return;
        }

        KLog::Log::info("Creating abstract device type " + std::to_string(_type) + " facing " + std::to_string(_face) + " at port " + std::to_string(_port));

        #ifdef STRICT_DEVICE_ASSIGNMENT
          const pros::c::v5_device_e_t portInfo = pros::c::registry_get_plugged_type(_port);

          if (portInfo != pros::c::E_DEVICE_NONE) {
            throw new PortOccupiedError(_port);
          } else if ((int) portInfo != (int) _type) {
            throw new UnexpectedDeviceFoundError(portInfo, _type);
          }
        #endif
      };

      /*
        Gets the enum type of the class

        @return Type of device
      */
      inline virtual device_types classname() const { return _type; }

      /*
        Get direction device is facing

        @return Direction the device is facing
      */
      inline virtual device_face facing() const { return _face; }

      /*
        Get port device is connected to

        @return Port the device is on
      */
      inline virtual char port() const { return _port; }
  };

  class Button : public pros::ADIDigitalIn, public AbstractDevice {
    public:
      /*
        @param device
      */
      inline explicit Button(const abstract_device_struct &device) : pros::ADIDigitalIn(device.port), AbstractDevice(K_BUTTON, device.face, device.port) {};
  };

  class Color : public pros::Optical, public AbstractDevice {
    public:
      /*
        @param device
      */
      inline explicit Color(const abstract_device_struct &device) : pros::Optical(device.port), AbstractDevice(K_COLOR, device.face, device.port) {};
  };

  class Controller : public pros::Controller, public AbstractDevice {
    protected:
      const pros::controller_id_e_t _id;
    public:
      /*
        @param controller
      */
      inline explicit Controller(const controller_struct &controller) : pros::Controller(controller.id), AbstractDevice(K_CONTROLLER, K_NONE, '\0'), _id(controller.id) {};

      /*
        Get controller id

        @return Controller id
      */
      inline pros::controller_id_e_t id() const { return _id; }

      /*
        Sets the controllers display screen text

        @param text Text to display to controller screen
      */
      inline void set_text(const std::string &text) {
        pros::Controller::clear();
        pros::Controller::set_text(0, 0, text + "         ");

        KLog::Log::info("Controller " + std::to_string(id()) + ": " + text);
      }
  };

  class Motor : public pros::Motor, public KPID::PID, public AbstractDevice {
    public:
      /*
        @param device
      */
      inline explicit Motor(const motor_struct &device) : pros::Motor(device.port, device.gearset, device.reverse, device.encoder), KPID::PID(device.pidexit, device.pidmods), AbstractDevice(K_MOTOR, device.face, device.port) {
        pros::Motor::set_brake_mode(device.brakemode);
      };

      /*
        Moves the motor to the target location

        @param target Target position of the motor

        @return If the PID loop exists and is set to 0
      */
      inline bool move_position_pid(const double &target) {
        const double velocity = KPID::PID::pid(target, get_position());

        pros::Motor::move_velocity(velocity);

        return !velocity;
      }

      /*
        Moves the motor to the target velocity

        @param target Target velocity of the motor

        @return If the PID loop exists and is set to 0
      */
      inline bool move_velocity_pid(const double &target) {
        const double velocity = KPID::PID::pid(target, get_actual_velocity());

        pros::Motor::move_velocity(velocity);

        return !velocity;
      }
  };

  class Piston : public pros::ADIDigitalOut, public AbstractDevice {
    protected:
      bool _value;
    public:
      /*
        @param device
      */
      inline explicit Piston(const abstract_device_struct &device) : pros::ADIDigitalOut(device.port), AbstractDevice(K_PISTON, device.face, device.port), _value(false) {};

      /*
        Sets the value of the piston

        @param setValue State to set the piston to

        @return The value that the piston is set to
      */
      inline bool set_value(const bool &setValue) {
        pros::ADIDigitalOut::set_value(setValue);
        _value = setValue;

        return _value;
      }

      /*
        Toggles the piston

        @return Value of the piston's new state
      */
      inline bool toggle() { return Piston::set_value(!_value); }

      /*
        Gets the piston's current value

        @return Piston's current state
      */
      inline bool value() const { return _value; }
  };

  class Proximity : public pros::Distance, public AbstractDevice {
    public:
      /*
        @param device
      */
      inline explicit Proximity(const abstract_device_struct &device) : pros::Distance(device.port), AbstractDevice(K_PROXIMITY, device.face, device.port) {};
  };

  class Vision : pros::Vision, public AbstractDevice {
    protected:
      std::map<int, pros::vision_signature_s_t> _signatures;
    public:
      /*
        @param device
      */
      inline explicit Vision(const abstract_device_struct &device) : pros::Vision(device.port), AbstractDevice(K_VISION, device.face, device.port) {};

      /*
        Save a vision signature to the vision sensor.

        @param name Name of the signature to add
        @param signature Vision signature to add
      */
      inline void addSignature(const int &name, const pros::vision_signature_s_t &signature) { _signatures.insert({name, signature}); }

      /*
        Get a vision signature saved from the sensor.

        @param name Name of signature to get

        @return Signature gotten from name
      */
      inline pros::vision_signature_s_t &getSignature(const int &name) { return _signatures.at(name); }

      /*
        Set the signature for the vision sensor.

        @param name Name of signature to set vision sensor to
      */
      inline void setSignature(const int &name) { set_signature(name, &(getSignature(name))); }
  };
}

#endif
