#include "robot.hpp"

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

template<class C>
Robot& Robot::linkController(std::map<C, std::string> &typeMap, C controls, std::string deviceName) {
    typeMap.insert({controls, deviceName});

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

Robot& Robot::pairDevices(const std::vector<std::string>& callNames, const std::string& pairName) {
    devicePairs.insert({pairName, callNames});

    return *this;
}

template<class T>
std::vector<Device<T>> Robot::getPairs(const device_types type, const std::string& name) {
    std::vector<Device<T>> pairs;

    for (std::string deviceName : devicePairs.at(name))
        switch (type) {
            case MOTOR:
                pairs.push_back(getMotor(deviceName));
                break;
            case PISTON:
                pairs.push_back(getPiston(deviceName));
                break;
        }

    return pairs;
}

void Robot::movePairMotors(const std::string& pairName, int32_t velocity) {
    for (std::string name : devicePairs.at(pairName))
        getMotor(name).move_velocity(velocity);
}

void Robot::movePairMotors(const std::string& pairName, int32_t velocity, double distance) {
    for (const std::string& name : devicePairs.at(pairName))
        getMotor(name).move_relative(distance, velocity);
}

Robot& Robot::linkDeviceController(pros::controller_analog_e_t control, std::string deviceName) {
    return linkController(controllerLinkAnalog, control, deviceName);
}

Robot& Robot::linkDeviceController(pros::controller_digital_e_t control, std::string deviceName) {
    return linkController(controllerLinkDigital, control, deviceName);
}

void Robot::controllerListener() {
    for (const auto& [controlType, devicePair] : controllerLinkDigital)
        if (master.get_digital(controlType));
}