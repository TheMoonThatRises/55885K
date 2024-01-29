/*
  Copyright 2024 Peter Duanmu

  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef KRONOS_BASE_DEVICES_HPP_
#define KRONOS_BASE_DEVICES_HPP_

#include <map>
#include <string>
#include <optional>

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

namespace KRONOS {
class AbstractDevice : public pros::Mutex {
 private:
    inline std::string _get_info() {
      std::string facing_msg = _face.has_value()
        ? " facing '" + std::to_string(_face.value())  + "'"
        : "";
      std::string port_msg = _port.has_value()
        ? " at port '" + std::to_string(_port.value()) + "'"
        : "";

      return "abstract device type '" + std::to_string(_type) + "'" +
             facing_msg +
             port_msg;
    }

    inline void _init() {
      KLog::Log::info("Constructing " + _get_info());

      if (_port.has_value()) {
        const pros::c::v5_device_e_t port_info =
          pros::c::registry_get_plugged_type(_port.value() - 1);

        int port_index = isdigit(_port.value())
          ? _port.value() - '0'
          : 21 + toupper(_port.value()) - 'A';

        if (port_info == pros::c::E_DEVICE_NONE) {
          return KLog::Log::warn(
            "No device found at port '" + std::to_string(_port.value()) + "'");
        } else if (port_info == pros::c::E_DEVICE_UNDEFINED) {
          return KLog::Log::warn(
            "Unknown device found at port '" +
            std::to_string(_port.value()) + "'");
        } else if (_occupied_ports[port_index] == 1) {
          throw new PortOccupiedError(_port.value());
        } else if (static_cast<int>(port_info) != static_cast<int>(_type)) {
          throw new UnexpectedDeviceFoundError(
            port_info,
            _type,
            _port.value());
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
    inline AbstractDevice(
      const device_types &device,
      const device_face &face,
      const char &port)
    : Mutex(), _type(device), _face(face), _port(port) {
      _init();
    }

    /*
      @param device
      @param face
    */

    inline AbstractDevice(
      const device_types &device,
      const device_face &face)
    : _type(device), _face(face) {
      _init();
    }
    /*
      @param device
      @param port
    */
    inline AbstractDevice(
      const device_types &device,
      const char &port)
    : _type(device), _port(port) {
      _init();
    }

    /*
      @param device
    */
    inline explicit AbstractDevice(const device_types &device)
    : _type(device) {
      _init();
    }

    virtual inline ~AbstractDevice() {
      KLog::Log::info("Destructuring " + _get_info());
    }

    /*
      Gets the enum type of the class

      @return Type of device
    */
    inline virtual device_types type() const {
      return _type;
    }

    /*
      Get direction device is facing

      @return Direction the device is facing
    */
    inline virtual std::optional<device_face> facing() const {
      return _face;
    }

    /*
      Get port device is connected to

      @return Port the device is on
    */
    inline virtual std::optional<char> port() const {
      return _port;
    }

    /*
      Tests if the device type is that inputted

      @param type The type to compare against

      @return If the device is of inputted type
    */
    inline virtual bool is_type(const device_types &type) const {
      return _type == type;
    }

    /*
      Takes mutex

      @return Successful operation or not
    */
    inline virtual bool mutex_take(
      const uint32_t &delay = KUtil::KRONOS_MSDELAY
    ) {
      #ifdef KRONOS_DEVICE_USE_MUTEX
        if (!Mutex::take(delay)) {
          #ifdef KRONOS_STRICT_MUTEX
            throw new UnsuccessfulMutexTake();
          #else
            KLog::Log::warn(
              "Unable to take mutex for device '" +
              std::to_string(_type) + "' on port '" +
              std::to_string(port().value()) +
              "'. This may cause unexpected device functionality. Errno: " +
              std::to_string(errno));
            return false;
          #endif
        }
      #else
        (void) delay;
      #endif

      return true;
    }
};

class Button : public pros::ADIDigitalIn, public AbstractDevice {
 public:
    /*
      @param device
    */
    inline explicit Button(const abstract_device_struct &device)
    : pros::ADIDigitalIn(device.port),
      AbstractDevice(K_BUTTON, device.face, device.port) {}
};

class Color : public pros::Optical, public AbstractDevice {
 public:
    /*
      @param device
    */
    inline explicit Color(const abstract_device_struct &device)
    : pros::Optical(device.port),
      AbstractDevice(K_COLOR, device.face, device.port) {}
};

class Controller : public pros::Controller, public AbstractDevice {
 private:
    const pros::controller_id_e_t _id;

    inline void logger(
      const KLog::log_types &log_type,
      const std::string &text) {
      KLog::Log::log(
        log_type,
        "Controller " + std::to_string(id()) + ": " + text);
    }

 public:
    /*
      @param controller
    */
    inline explicit Controller(const controller_struct &controller)
    : pros::Controller(controller.id),
      AbstractDevice(K_CONTROLLER),
      _id(controller.id) {}

    /*
      Get controller id

      @return Controller id
    */
    inline pros::controller_id_e_t id() const {
      return _id;
    }

    /*
      Rumbles the controller in the pattern given. Wrapper for pros::Controller::rumble

      @param pattern The pattern to rumble the controller
    */
    inline void rumble(const std::string &pattern) {
      (void) AbstractDevice::mutex_take();

      (void) pros::Controller::rumble(pattern.c_str());

      logger(KLog::log_types::L_WARNING, "Rumbling pattern: " + pattern);
    }

    /*
      Sets the controllers display screen text

      @param text Text to display to controller screen
    */
    inline void set_text(const std::string &text) {
      (void) AbstractDevice::mutex_take();

      // pros::Controller::clear();
      (void) pros::Controller::set_text(0, 0, text + "         ");

      logger(KLog::log_types::L_INFO, text);
    }
};

class Imu : public pros::Imu, public AbstractDevice {
 public:
    inline explicit Imu(const abstract_device_struct &device)
    : pros::Imu(device.port),
      AbstractDevice(K_IMU, device.port) {
      (void) pros::Imu::reset();
    }
};

class Motor
: public pros::Motor,
  public KExtender::PID,
  public AbstractDevice {
 public:
    /*
      @param device
    */
    inline explicit Motor(const motor_struct &device)
    : pros::Motor(device.port, device.gearset, device.reverse, device.encoder),
      KExtender::PID(device.pidexit, device.pidmods, device.consistencymods),
      AbstractDevice(K_MOTOR, device.face, device.port) {
      (void) pros::Motor::set_brake_mode(device.brakemode);
    }

    /*
      Moves the motor to the target location

      @param target Target position of the motor

      @return If the PID loop exists and is set to 0
    */
    inline bool move_position_pid(const double &target) {
      (void) AbstractDevice::mutex_take();

      const double velocity = KExtender::PID::tick(target, get_position());

      (void) pros::Motor::move_velocity(velocity);

      return velocity == 0;
    }

    /*
      Moves the motor to the target velocity

      @param target Target velocity of the motor

      @return Velocity pid is set to
    */
    inline double move_velocity_pid(const double &target) {
      (void) AbstractDevice::mutex_take();

      const double velocity =
        KExtender::PID::tick(target, get_actual_velocity());

      (void) pros::Motor::move_velocity(velocity);

      return velocity;
    }

    /*
      Moves motor to velocity. Wrapper for pros::Motor::move_velocity

      @param velocity Velocity to set motor to
    */
    inline int32_t move_velocity(const double &velocity) {
      (void) AbstractDevice::mutex_take();

      return pros::Motor::move_velocity(velocity);
    }

    /*
      Moves motor to velocity with an optional sleep parameter

      @param velocity Velocity to set motor to
      @param sleep How long to sleep afterwards
    */
    inline void move_velocity(const double &velocity, const double &sleep) {
      (void) AbstractDevice::mutex_take(sleep);

      (void) pros::Motor::move_velocity(velocity);
      pros::delay(sleep);
      (void) pros::Motor::move_velocity(0);
    }
};

class PIDDevice : public KExtender::PID, public AbstractDevice {
 public:
    inline explicit PIDDevice(
      const KExtender::pid_exit_conditions &exitcon,
      const KExtender::pid_consts &pidconsts,
      const KExtender::consistency_consts &consistencyconsts)
    : KExtender::PID(exitcon, pidconsts, consistencyconsts),
      AbstractDevice(K_PID) {}

    inline double tick(const double &target, const double &current) override {
      (void) AbstractDevice::mutex_take();

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
    inline explicit Piston(const abstract_device_struct &device)
    : pros::ADIDigitalOut(device.port),
      AbstractDevice(K_PISTON, device.face, device.port),
      _value(false) {}

    /*
      Sets the value of the piston

      @param setValue State to set the piston to

      @return The value that the piston is set to
    */
    inline bool set_value(const bool &setValue) {
      (void) AbstractDevice::mutex_take();

      (void) pros::ADIDigitalOut::set_value(setValue);
      _value = setValue;

      return _value;
    }

    /*
      Toggles the piston

      @return Value of the piston's new state
    */
    inline bool toggle() {
      return Piston::set_value(!_value);
    }

    /*
      Gets the piston's current value

      @return Piston's current state
    */
    inline bool value() const {
      return _value;
    }
};

class Proximity : public pros::Distance, public AbstractDevice {
 public:
    /*
      @param device
    */
    inline explicit Proximity(const abstract_device_struct &device)
    : pros::Distance(device.port),
      AbstractDevice(K_PROXIMITY, device.face, device.port) {}
};

class Rotation : public pros::Rotation, public AbstractDevice {
 public:
  /*
    @param device
  */
  inline explicit Rotation(const abstract_device_struct &device)
  : pros::Rotation(device.port, device.reverse),
    AbstractDevice(K_ROTATION, device.face, device.port) {}
};

class Vision : public pros::Vision, public AbstractDevice {
 private:
    std::map<int, pros::vision_signature_s_t> _signatures;

 public:
    /*
      @param device
    */
    inline explicit Vision(const abstract_device_struct &device)
    : pros::Vision(device.port),
      AbstractDevice(K_VISION, device.face, device.port) {
      (void) pros::Vision::set_zero_point(pros::E_VISION_ZERO_CENTER);
    }

    inline Vision& set_zero_point(const pros::vision_zero_e_t &zero_point) {
      (void) AbstractDevice::mutex_take();

      (void) pros::Vision::set_zero_point(zero_point);

      return *this;
    }

    /*
      Save a vision signature to the vision sensor.

      @param name Name of the signature to add
      @param signature Vision signature to add

      @return Self to chain
    */
    inline Vision& add_signature(
      const int &name,
      const pros::vision_signature_s_t &signature
    ) {
      (void) _signatures.insert({name, signature});

      return *this;
    }

    /*
      Get a vision signature saved from the sensor.

      @param name Name of signature to get

      @return Signature gotten from name
    */
    inline pros::vision_signature_s_t &get_signature(const int &name) {
      return _signatures.at(name);
    }

    /*
      Set the signature for the vision sensor.

      @param name Name of signature to set vision sensor to

      @return Self to chain
    */
    inline Vision& set_signature(const int &name) {
      (void) AbstractDevice::mutex_take();

      (void) pros::Vision::set_signature(name, &(get_signature(name)));

      return *this;
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_DEVICES_HPP_
