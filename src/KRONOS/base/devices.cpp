/*
  Copyright 2025 Peter Duanmu

  @file base/device.cpp

  Device wrapper for KRONOS
*/

#include <string>
#include <optional>

#include "KRONOS/base/devices.hpp"

namespace kronos::base {
using assets::Logger;
using assets::PortOccupiedError;
using assets::UnexpectedDeviceFoundError;
using assets::device_types;
using assets::device_face;
using assets::abstract_device_struct;
using assets::log_types;
using assets::controller_struct;
using assets::motor_struct;
using assets::pid_consts;
using assets::consistency_consts;
using assets::pid_exit_conditions;

using extenders::PID;

std::string AbstractDevice::_get_info() {
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

void AbstractDevice::_init() {
  Logger::info("Constructing " + _get_info());

  if (_port.has_value()) {
    const pros::c::v5_device_e_t port_info =
      pros::c::registry_get_plugged_type(_port.value() - 1);

    int port_index = isdigit(_port.value())
      ? _port.value() - '0'
      : 21 + toupper(_port.value()) - 'A';

    if (port_info == pros::c::E_DEVICE_NONE) {
      return Logger::warn(
        "No device found at port '" + std::to_string(_port.value()) + "'");
    } else if (port_info == pros::c::E_DEVICE_UNDEFINED) {
      return Logger::warn(
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

AbstractDevice::AbstractDevice(
  const device_types& device,
  const device_face& face,
  const int8_t& port)
: Mutex(), _type(device), _face(face), _port(port) {
  _init();
}

AbstractDevice::AbstractDevice(
  const device_types& device,
  const device_face& face)
: _type(device), _face(face) {
  _init();
}
AbstractDevice::AbstractDevice(
  const device_types& device,
  const char& port)
: _type(device), _port(port) {
  _init();
}

AbstractDevice::AbstractDevice(const device_types& device)
: _type(device) {
  _init();
}

AbstractDevice::~AbstractDevice() {
  Logger::info("Destructuring " + _get_info());
}

device_types AbstractDevice::type() const {
  return _type;
}

std::optional<device_face> AbstractDevice::facing() const {
  return _face;
}

std::optional<int8_t> AbstractDevice::port() const {
  return _port;
}

bool AbstractDevice::is_type(const device_types& type) const {
  return _type == type;
}

bool AbstractDevice::mutex_take(const uint32_t& delay) {
  #ifdef KRONOS_DEVICE_USE_MUTEX
    if (!Mutex::take(delay)) {
      #ifdef KRONOS_STRICT_MUTEX
        throw new UnsuccessfulMutexTake();
      #else
        Logger::warn(
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

Button::Button(const abstract_device_struct& device)
: pros::adi::Button(device.port),
AbstractDevice(device_types::K_BUTTON, device.face, device.port) {}

Color::Color(const abstract_device_struct& device)
: pros::Optical(device.port),
AbstractDevice(device_types::K_COLOR, device.face, device.port) {}

void Controller::logger(
  const log_types& log_type,
  const std::string& text) {
  Logger::log(
    log_type,
    "Controller " + std::to_string(id()) + ": " + text);
}

Controller::Controller(const controller_struct& controller)
: pros::Controller(controller.id),
AbstractDevice(device_types::K_CONTROLLER),
_id(controller.id) {}

pros::controller_id_e_t Controller::id() const {
  return _id;
}

void Controller::rumble(const std::string& pattern) {
  (void) AbstractDevice::mutex_take();

  (void) pros::Controller::rumble(pattern.c_str());

  logger(log_types::L_WARNING, "Rumbling pattern: " + pattern);
}

void Controller::set_text(const std::string& text) {
  (void) AbstractDevice::mutex_take();

  // pros::Controller::clear();
  (void) pros::Controller::set_text(0, 0, text + "         ");

  logger(log_types::L_INFO, text);
}

Imu::Imu(const abstract_device_struct& device)
: pros::Imu(device.port),
AbstractDevice(device_types::K_IMU, device.port) {
  (void) pros::Imu::reset();
}

Motor::Motor(const motor_struct& device)
: pros::Motor(device.port, device.gearset, device.encoder),
PID(device.pidexit, device.pidmods, device.consistencymods),
AbstractDevice(device_types::K_MOTOR, device.face, device.port) {
  (void) pros::Motor::set_brake_mode(device.brakemode);
}

bool Motor::move_position_pid(const double& target) {
  (void) AbstractDevice::mutex_take();

  const double velocity = PID::tick(target, get_position());

  (void) pros::Motor::move_velocity(velocity);

  return velocity == 0;
}

double Motor::move_velocity_pid(const double& target) {
  (void) AbstractDevice::mutex_take();

  const double velocity =
    PID::tick(target, get_actual_velocity());

  (void) pros::Motor::move_velocity(velocity);

  return velocity;
}

int32_t Motor::move_velocity(const double& velocity) {
  (void) AbstractDevice::mutex_take();

  return pros::Motor::move_velocity(velocity);
}

void Motor::move_velocity(const double& velocity, const double& sleep) {
  (void) AbstractDevice::mutex_take(sleep);

  (void) pros::Motor::move_velocity(velocity);
  pros::delay(sleep);
  (void) pros::Motor::move_velocity(0);
}

PIDDevice::PIDDevice(
  const pid_exit_conditions& exitcon,
  const pid_consts& pidconsts,
  const consistency_consts& consistencyconsts)
: PID(exitcon, pidconsts, consistencyconsts),
AbstractDevice(device_types::K_PID) {}

double PIDDevice::tick(const double& target, const double& current) {
  (void) AbstractDevice::mutex_take();

  return PID::tick(target, current);
}

Piston::Piston(const abstract_device_struct& device)
: pros::adi::Pneumatics(
  device.port,
  device.start_retracted,
  device.reverse),
AbstractDevice(device_types::K_PISTON, device.face, device.port) {}

Proximity::Proximity(const abstract_device_struct& device)
: pros::Distance(device.port),
AbstractDevice(device_types::K_PROXIMITY, device.face, device.port) {}

Rotation::Rotation(const abstract_device_struct& device)
: pros::Rotation(device.port),
AbstractDevice(device_types::K_ROTATION, device.face, device.port) {}

double Rotation::get_angle_degrees() {
  return get_angle() / 100.0;
}

Vision::Vision(const abstract_device_struct& device)
: pros::Vision(device.port),
AbstractDevice(device_types::K_VISION, device.face, device.port) {
  (void) pros::Vision::set_zero_point(pros::E_VISION_ZERO_CENTER);
}

Vision& Vision::set_zero_point(const pros::vision_zero_e_t& zero_point) {
  (void) AbstractDevice::mutex_take();

  (void) pros::Vision::set_zero_point(zero_point);

  return *this;
}

Vision& Vision::add_signature(
  const int& name,
  const pros::vision_signature_s_t& signature
) {
  (void) _signatures.insert({name, signature});

  return *this;
}

pros::vision_signature_s_t& Vision::get_signature(const int& name) {
  return _signatures.at(name);
}

Vision& Vision::set_signature(const int& name) {
  (void) AbstractDevice::mutex_take();

  (void) pros::Vision::set_signature(name, &get_signature(name));

  return *this;
}

GPS::GPS(const abstract_device_struct& device)
: pros::GPS(device.port),
AbstractDevice(device_types::K_GPS, device.port) {}

LineTracker::LineTracker(const abstract_device_struct& device)
: pros::ADIAnalogIn(device.port),
AbstractDevice(device_types::K_LINE_TRACKER, device.port) {}
}  // namespace kronos::base
