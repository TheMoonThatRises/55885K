#include "robot.hpp"
#include <stdexcept>

using namespace KRONOS;

Robot::Robot(std::vector<Device<Motor>> motorObject, std::vector<Device<Piston>> pistonObject, std::map<std::string, pros::ADIDigitalIn> buttonMap):
    master(pros::E_CONTROLLER_MASTER)
{
    if (motorObject.size() > 0) for (Device<Motor> motor : motorObject) motors.push_back(motor);
    if (pistonObject.size() > 0) for (Device<Piston> piston : pistonObject) pistons.push_back(piston);
    if (buttonMap.size() > 0) for (const auto [name, button] : buttonMap) buttons.insert({name, button});
}

template<class T>
T Robot::getType(std::vector<Device<T>> devices, std::string name) {
    for (Device<T> device : devices) 
        if (name.length() == 2 && device.callsign == name) return device.device;
        else if (device.name == name) return device.device;

    throw std::runtime_error("Device not found. " + __FILE__ + " (" + __LINE__ + ")");
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

Controller Robot::getController() {
    return master;
}