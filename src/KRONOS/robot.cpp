#include "main.h"
#include "device.hpp"
#include "motor.hpp"
#include "piston.hpp"
#include "robot.hpp"
#include <vector>
#include <string>

using namespace KRONOS;

Robot::Robot(std::vector<Device<Motor>> motorObject):
    master(pros::E_CONTROLLER_MASTER)
{
    if (motorObject.size() > 0) for (Device<Motor> motor : motorObject) motors.push_back(motor);
}

template<class T>
T Robot::getType(std::vector<Device<T>> devices, std::string name) {
    for (Device<T> device : devices) if (name.length() == 2 && device.callsign == name) return device.device;
                                     else if (device.name == name) return device.device;
}

Motor Robot::getMotor(std::string name) {
    return getType(motors, name);
}

Piston Robot::getPiston(std::string name) {
    return getType(pistons, name);
}

pros::ADIDigitalIn Robot::getButton(std::string name) {
    return buttons.at(name);
}

pros::Controller Robot::getController() {
    return master;
}