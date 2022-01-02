#include "robot.hpp"

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ " (" S2(__LINE__) ")"

using namespace KRONOS;

Robot::Robot():
    master(pros::E_CONTROLLER_MASTER)
{

}

template<class T>
T Robot::getType(std::vector<Device<T>> devices, std::string name) {
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
    return addType(motors, motor);
}

Robot& Robot::addPiston(Device<Piston> piston) {
    return addType(pistons, piston);
}

Robot& Robot::addButton(std::string name, pros::ADIDigitalIn button) {
    buttons.insert({name, button});

    return *this;
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

void Robot::pairMotors(std::vector<std::string> callNames, std::string pairName) {
    motorPairs.insert({pairName, callNames});
}

void Robot::movePair(std::string pairName, int32_t velocity) {
    for (std::string name : motorPairs.at(pairName))
        getMotor(name).move_velocity(velocity);
}

void Robot::movePair(std::string pairName, int32_t velocity, double distance) {
    for (std::string name : motorPairs.at(pairName))
        getMotor(name).move_relative(distance, velocity);
}