#include "robot.hpp"

#include <utility>

using namespace KRONOS;

Robot::Robot():
    master(pros::E_CONTROLLER_MASTER)
{

}

template<class T>
T Robot::getType(std::vector<Device<T>> devices, const std::string& name) {
    for (Device<T> device : devices) 
        if (name.length() == 2 && device.callsign == name) return device.device;
        else if (device.name == name) return device.device;

    throw std::runtime_error("Device not found. " + std::string(LOCATION));
}

template<class T>
Robot& Robot::addType(std::vector<Device<T>> &devices, Device<T> device) {
    devices.push_back(device);

    return *this;
}

Robot& Robot::addMotor(Device<Motor> motor) {
    return addType(motors, std::move(motor));
}

Robot& Robot::addPiston(Device<Piston> piston) {
    return addType(pistons, std::move(piston));
}

Robot& Robot::addButton(const std::string& name, pros::ADIDigitalIn button) {
    buttons.insert({name, button});

    return *this;
}

Motor Robot::getMotor(const std::string& name) {
    return getType(motors, name);
}

Piston Robot::getPiston(const std::string& name) {
    return getType(pistons, name);
}

pros::ADIDigitalIn Robot::getButton(const std::string& name) {
    return buttons.at(name);
}

Controller Robot::getController() {
    return master;
}

void Robot::pairMotors(const std::vector<std::string>& callNames, const std::string& pairName) {
    motorPairs.insert({pairName, callNames});
}

void Robot::movePair(const std::string& pairName, int32_t velocity) {
    for (std::string name : motorPairs.at(pairName))
        getMotor(name).move_velocity(velocity);
}

void Robot::movePair(const std::string& pairName, int32_t velocity, double distance) {
    for (const std::string& name : motorPairs.at(pairName))
        getMotor(name).move_relative(distance, velocity);
}

void Robot::controllerListener() {

}