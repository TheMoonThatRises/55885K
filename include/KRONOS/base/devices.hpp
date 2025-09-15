/*
  Copyright 2025 Peter Duanmu

  @file base/device.hpp

  Device wrapper for KRONOS
*/

#ifndef KRONOS_BASE_DEVICES_HPP_
#define KRONOS_BASE_DEVICES_HPP_

#include <string>
#include <map>

#include "KRONOS/assets/structs.hpp"
#include "KRONOS/assets/errors.hpp"
#include "KRONOS/assets/logger.hpp"
#include "KRONOS/assets/statics.hpp"

#include "KRONOS/base/extenders/pid.hpp"

#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "pros/rtos.hpp"

namespace kronos::base {
class AbstractDevice : public pros::rtos::Mutex {
 private:
    const assets::device_types _type;
    const std::optional<assets::device_face> _face;
    const std::optional<int8_t> _port;

    inline static char _occupied_ports[29] {};

 private:
    std::string _get_info();
    void _init();

 public:
    /*
      @param device
      @param face
      @param port
    */
    AbstractDevice(
      const assets::device_types& device,
      const assets::device_face& face,
      const int8_t& port);

    /*
      @param device
      @param face
    */

    AbstractDevice(
      const assets::device_types& device, const assets::device_face& face);

    /*
      @param device
      @param port
    */
    AbstractDevice(const assets::device_types& device, const char& port);

    /*
      @param device
    */
    explicit AbstractDevice(const assets::device_types& device);

    virtual ~AbstractDevice();

    /*
      Gets the enum type of the class

      @return Type of device
    */
    virtual assets::device_types type() const;

    /*
      Get direction device is facing

      @return Direction the device is facing
    */
    virtual std::optional<assets::device_face> facing() const;

    /*
      Get port device is connected to

      @return Port the device is on
    */
    virtual std::optional<int8_t> port() const;

    /*
      Tests if the device type is that inputted

      @param type The type to compare against

      @return If the device is of inputted type
    */
    virtual bool is_type(const assets::device_types& type) const;

    /*
      Takes mutex

      @return Successful operation or not
    */
    virtual bool mutex_take(const uint32_t& delay = assets::KRONOS_MSDELAY);
};

class Button : public pros::adi::Button, public AbstractDevice {
 public:
    /*
      @param device
    */
    explicit Button(const assets::abstract_device_struct& device);
};

class Color : public pros::Optical, public AbstractDevice {
 public:
    /*
      @param device
    */
    explicit Color(const assets::abstract_device_struct& device);
};

class Controller : public pros::Controller, public AbstractDevice {
 private:
    const pros::controller_id_e_t _id;

    void logger(const assets::log_types& log_type, const std::string& text);

 public:
    /*
      @param controller
    */
    explicit Controller(const assets::controller_struct& controller);

    /*
      Get controller id

      @return Controller id
    */
    pros::controller_id_e_t id() const;

    /*
      Rumbles the controller in the pattern given. Wrapper for pros::Controller::rumble

      @param pattern The pattern to rumble the controller
    */
    void rumble(const std::string& pattern);

    /*
      Sets the controllers display screen text

      @param text Text to display to controller screen
    */
    void set_text(const std::string& text);
};

class Imu : public pros::Imu, public AbstractDevice {
 public:
    explicit Imu(const assets::abstract_device_struct& device);
};

class Motor
: public pros::Motor,
  public extenders::PID,
  public AbstractDevice {
 private:
  using pros::Motor::move_velocity;

 public:
    /*
      @param device
    */
    explicit Motor(const assets::motor_struct& device);

    /*
      Moves the motor to the target location

      @param target Target position of the motor

      @return If the PID loop exists and is set to 0
    */
    bool move_position_pid(const double& target);

    /*
      Moves the motor to the target velocity

      @param target Target velocity of the motor

      @return Velocity pid is set to
    */
    double move_velocity_pid(const double& target);

    /*
      Moves motor to velocity. Wrapper for pros::Motor::move_velocity

      @param velocity Velocity to set motor to
    */
    int32_t move_velocity(const double& velocity);

    /*
      Moves motor to velocity with an optional sleep parameter

      @param velocity Velocity to set motor to
      @param sleep How long to sleep afterwards
    */
    void move_velocity(const double& velocity, const double& sleep);
};

class PIDDevice : public extenders::PID, public AbstractDevice {
 public:
    explicit PIDDevice(
      const assets::pid_exit_conditions& exitcon,
      const assets::pid_consts& pidconsts,
      const assets::consistency_consts& consistencyconsts);

    double tick(const double& target, const double& current) override;
};

class Piston : public pros::adi::Pneumatics, public AbstractDevice {
 public:
    /*
      @param device
    */
    explicit Piston(const assets::abstract_device_struct& device);
};

class Proximity : public pros::Distance, public AbstractDevice {
 public:
    /*
      @param device
    */
    explicit Proximity(const assets::abstract_device_struct& device);
};

class Rotation : public pros::Rotation, public AbstractDevice {
 public:
  /*
    @param device
  */
  explicit Rotation(const assets::abstract_device_struct& device);

  /*
    Gets position in degrees
  */
  double get_angle_degrees();
};

class Vision : public pros::Vision, public AbstractDevice {
 private:
    std::map<int, pros::vision_signature_s_t> _signatures;

 public:
    /*
      @param device
    */
    explicit Vision(const assets::abstract_device_struct& device);

    Vision& set_zero_point(const pros::vision_zero_e_t& zero_point);

    /*
      Save a vision signature to the vision sensor.

      @param name Name of the signature to add
      @param signature Vision signature to add

      @return Self to chain
    */
    Vision& add_signature(
      const int& name,
      const pros::vision_signature_s_t& signature);

    /*
      Get a vision signature saved from the sensor.

      @param name Name of signature to get

      @return Signature gotten from name
    */
    pros::vision_signature_s_t& get_signature(const int& name);

    /*
      Set the signature for the vision sensor.

      @param name Name of signature to set vision sensor to

      @return Self to chain
    */
    Vision& set_signature(const int& name);
};

class GPS : public pros::GPS, public AbstractDevice {
 public:
    explicit GPS(const assets::abstract_device_struct& device);
};

class LineTracker : public pros::ADIAnalogIn, public AbstractDevice {
 public:
  explicit LineTracker(const assets::abstract_device_struct& device);
};
}  // namespace kronos::base

#endif  // KRONOS_BASE_DEVICES_HPP_
