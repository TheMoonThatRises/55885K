/*
  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef _DEVICES_HPP_
#define _DEVICES_HPP_

#include "assets/devicestructs.hpp"
#include "assets/errortypes.hpp"
#include "assets/logger.hpp"
#include "assets/statics.hpp"

#include "base/extenders/pid.hpp"

#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "pros/rtos.hpp"

#include <map>
#include <optional>

namespace KRONOS {
  class AbstractDevice : public pros::Mutex {
    private:
      inline std::string _get_info() {
        return "abstract device type '" + std::to_string(_type) + "'" + (_face.has_value() ? " facing '" + std::to_string(_face.value())  + "'": "") + (_port.has_value() ? " at port '" + std::to_string(_port.value()) + "'" : "");
      }

      inline void _init() {
        KLog::Log::info("Constructing " + _get_info());

        if (_port.has_value()) {
          const pros::c::v5_device_e_t portInfo = pros::c::registry_get_plugged_type(_port.value());

          int port_index = isdigit(_port.value()) ? _port.value() - '0' : 21 + toupper(_port.value()) - 'A';

          if (portInfo == pros::c::E_DEVICE_NONE) {
            return KLog::Log::warn("No device found at port '" + std::to_string(_port.value()) + "'");
          } else if (portInfo == pros::c::E_DEVICE_UNDEFINED) {
            return KLog::Log::warn("Unknown device found at port '" + std::to_string(_port.value()) + "'");
          } else if (_occupied_ports[port_index] == 1) {
            throw new PortOccupiedError(_port.value());
          } else if ((int) portInfo != (int) _type) {
            throw new UnexpectedDeviceFoundError(portInfo, _type, _port.value());
          } else {
            _occupied_ports[port_index] = 1;
          }
        }
      }

      const device_types _type;
      const std::optional<device_face> _face;
      const std::optional<char> _port;

      inline static char _occupied_ports[29] {};
    public:
      /*
        @param device
        @param face
        @param port
      */
      inline AbstractDevice(const device_types &device, const device_face &face, const char &port) : Mutex(), _type(device), _face(face), _port(port) {
        _init();
      };

      /*
        @param device
        @param face
      */

      inline AbstractDevice(const device_types &device, const device_face &face) : _type(device), _face(face) {
        _init();
      };
      /*
        @param device
        @param port
      */
      inline AbstractDevice(const device_types &device, const char &port) : _type(device), _port(port) {
        _init();
      };

      /*
        @param device
      */
      inline explicit AbstractDevice(const device_types &device) : _type(device) {
        _init();
      };

      virtual inline ~AbstractDevice() {
        KLog::Log::info("Destructuring " + _get_info());
      };

      /*
        Gets the enum type of the class

        @return Type of device
      */
      inline virtual device_types type() const { return _type; }

      /*
        Get direction device is facing

        @return Direction the device is facing
      */
      inline virtual std::optional<device_face> facing() const { return _face; }

      /*
        Get port device is connected to

        @return Port the device is on
      */
      inline virtual std::optional<char> port() const { return _port; }

      /*
        Tests if the device type is that inputted

        @param type The type to compare against

        @return If the device is of inputted type
      */
      inline virtual bool is_type(const device_types &type) const { return _type == type; }

      /*
        Takes mutex

        @return Successful operation or not
      */
      inline virtual bool mutex_take(const uint32_t &delay = KUtil::KRONOS_MSDELAY) {
        // #ifdef KRONOS_DEVICE_USE_MUTEX
        //   if (!Mutex::take(delay)) {
        //     #ifdef KRONOS_STRICT_MUTEX
        //       throw new UnsuccessfulMutexTake();
        //     #else
        //       KLog::Log::warn("Unable to take mutex for device '" + std::to_string(_type) + "' on port '" + std::to_string(port().value()) + "'. This may cause unexpected device functionality. Errno: " + std::to_string(errno));
        //       return false;
        //     #endif
        //   }
        // #endif

        return true;
      }
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
    private:
      const pros::controller_id_e_t _id;

      inline void logger(const KLog::log_types &log_type, const std::string &text) {
        KLog::Log::log(log_type, "Controller " + std::to_string(id()) + ": " + text);
      }
    public:
      /*
        @param controller
      */
      inline explicit Controller(const controller_struct &controller) : pros::Controller(controller.id), AbstractDevice(K_CONTROLLER), _id(controller.id) {};

      /*
        Get controller id

        @return Controller id
      */
      inline pros::controller_id_e_t id() const { return _id; }

      /*
        Rumbles the controller in the pattern given. Wrapper for pros::Controller::rumble

        @param pattern The pattern to rumble the controller
      */
      inline void rumble(const std::string &pattern) {
        AbstractDevice::mutex_take();

        pros::Controller::rumble(pattern.c_str());

        logger(KLog::log_types::L_WARNING, "Rumbling pattern: " + pattern);
      }

      /*
        Sets the controllers display screen text

        @param text Text to display to controller screen
      */
      inline void set_text(const std::string &text) {
        AbstractDevice::mutex_take();

        // pros::Controller::clear();
        pros::Controller::set_text(0, 0, text + "         ");

        logger(KLog::log_types::L_INFO, text);
      }
  };

  class Imu : public pros::Imu, public AbstractDevice {
    public:
      inline explicit Imu(const abstract_device_struct &device) : pros::Imu(device.port), AbstractDevice(K_IMU, device.port) {
        pros::Imu::reset();
      };
  };

  class Motor : public pros::Motor, public KExtender::PID, public AbstractDevice {
    public:
      /*
        @param device
      */
      inline explicit Motor(const motor_struct &device) : pros::Motor(device.port, device.gearset, device.reverse, device.encoder), KExtender::PID(device.pidexit, device.pidmods, device.consistencymods), AbstractDevice(K_MOTOR, device.face, device.port) {
        pros::Motor::set_brake_mode(device.brakemode);
      };

      /*
        Moves the motor to the target location

        @param target Target position of the motor

        @return If the PID loop exists and is set to 0
      */
      inline bool move_position_pid(const double &target) {
        AbstractDevice::mutex_take();

        const double velocity = KExtender::PID::tick(target, get_position());

        pros::Motor::move_velocity(velocity);

        return velocity == 0;
      }

      /*
        Moves the motor to the target velocity

        @param target Target velocity of the motor

        @return Velocity pid is set to
      */
      inline double move_velocity_pid(const double &target) {
        AbstractDevice::mutex_take();

        const double velocity = KExtender::PID::tick(target, get_actual_velocity());

        pros::Motor::move_velocity(velocity);

        return velocity;
      }

      /*
        Moves motor to velocity. Wrapper for pros::Motor::move_velocity

        @param velocity Velocity to set motor to
      */
      inline int32_t move_velocity(const double &velocity) {
        AbstractDevice::mutex_take();

        return pros::Motor::move_velocity(velocity);
      }

      /*
        Moves motor to velocity with an optional sleep parameter

        @param velocity Velocity to set motor to
        @param sleep How long to sleep afterwards
      */
      inline void move_velocity(const double &velocity, const double &sleep) {
        AbstractDevice::mutex_take(sleep);

        pros::Motor::move_velocity(velocity);
        pros::delay(sleep);
        pros::Motor::move_velocity(0);
      }
  };

  class PIDDevice : public KExtender::PID, public AbstractDevice {
    public:
      inline explicit PIDDevice(const KExtender::pid_exit_conditions &exitcon, const KExtender::pid_consts &pidconsts, const KExtender::consistency_consts &consistencyconsts) : KExtender::PID(exitcon, pidconsts, consistencyconsts), AbstractDevice(K_PID) {};

      inline double tick(const double &target, const double &current) {
        AbstractDevice::mutex_take();

        return KExtender::PID::tick(target, current);
      }
  };

  class Piston : public pros::ADIDigitalOut, public AbstractDevice {
    private:
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
        AbstractDevice::mutex_take();

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

  class Vision : public pros::Vision, public AbstractDevice {
    private:
      std::map<int, pros::vision_signature_s_t> _signatures;
    public:
      /*
        @param device
      */
      inline explicit Vision(const abstract_device_struct &device) : pros::Vision(device.port), AbstractDevice(K_VISION, device.face, device.port) {
        pros::Vision::set_zero_point(pros::E_VISION_ZERO_CENTER);
      };

      inline Vision& set_zero_point(const pros::vision_zero_e_t &zero_point) {
        AbstractDevice::mutex_take();

        pros::Vision::set_zero_point(zero_point);

        return *this;
      }

      /*
        Save a vision signature to the vision sensor.

        @param name Name of the signature to add
        @param signature Vision signature to add

        @return Self to chain
      */
      inline Vision& add_signature(const int &name, const pros::vision_signature_s_t &signature) {
        _signatures.insert({name, signature});

        return *this;
      }

      /*
        Get a vision signature saved from the sensor.

        @param name Name of signature to get

        @return Signature gotten from name
      */
      inline pros::vision_signature_s_t &get_signature(const int &name) { return _signatures.at(name); }

      /*
        Set the signature for the vision sensor.

        @param name Name of signature to set vision sensor to

        @return Self to chain
      */
      inline Vision& set_signature(const int &name) {
        AbstractDevice::mutex_take();

        pros::Vision::set_signature(name, &(get_signature(name)));

        return *this;
      }
  };
}

#endif
