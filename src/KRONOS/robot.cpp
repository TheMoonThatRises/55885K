#include "robot.hpp"

using namespace KRONOS;

Robot::Robot(int wheelAddVelocity, int chassisSensitivity):
    wheelAddVelocity(wheelAddVelocity),
    chassisSensitivity(chassisSensitivity)
{

}

template<class T>
T& Robot::getType(std::vector<Device<T>>& devices, const std::string& name) {
    for (Device<T>& device : devices) 
        if ((name.length() == 2 && device.callsign == name) || device.name == name) 
            return device.device;

    throw std::runtime_error("Device not found. " + std::string(LOCATION));
}

template<class T>
Robot& Robot::addType(std::vector<Device<T>>& devices, const Device<T>& device) {
    devices.push_back(device);

    device_types devicetype;

    std::string name = typeid(T).name();
    std::transform(name.begin(), name.end(), name.begin(), [](const unsigned char& c) { return std::tolower(c); });

    if (name.find("piston") != std::string::npos)
        devicetype = KRONOS::PISTON;
    else if (name.find("motor") != std::string::npos)
        devicetype = KRONOS::MOTOR;

    deviceType.insert({device.callsign, devicetype});

    return *this;
}

std::vector<Device<Motor>>& Robot::getMotorVector() {
    return motors;
}

Robot& Robot::addMotor(const Device<Motor>& motor) {
    return addType(motors, motor);
}

Robot& Robot::addPiston(const Device<Piston>& piston) {
    return addType(pistons, piston);
}

Robot& Robot::addProximity(const Device<Proximity>& proximity) {
    return addType(proximities, proximity);
}

Robot& Robot::addVision(const Device<Vision>& vision) {
    return addType(visions, vision);
}

Robot& Robot::addButton(const std::string& name, const pros::ADIDigitalIn& button) {
    buttons.insert({name, button});

    return *this;
}

Motor& Robot::getMotor(const std::string& name) {
    return getType(motors, name);
}

Piston& Robot::getPiston(const std::string& name) {
    return getType(pistons, name);
}

Proximity& Robot::getProximity(const std::string& name) {
    return getType(proximities, name);
}

Vision& Robot::getVision(const std::string& name) {
    return getType(visions, name);
}

device_types Robot::getDeviceType(const std::string& callsign) {
    return deviceType.at(callsign);
}

pros::ADIDigitalIn Robot::getButton(const std::string& name) {
    return buttons.at(name);
}

Robot& Robot::pairDevices(const std::vector<std::string>& callNames, const std::string& pairName) {
    devicePairs.insert({pairName, callNames});

    return *this;
}

template<class T>
std::vector<Device<T>> Robot::getPairs(const device_types& type, const std::string& name) {
    std::vector<Device<T>> pairs;

    for (const std::string& deviceName : devicePairs.at(name))
        switch (type) {
            case MOTOR:
                pairs.push_back(getMotor(deviceName));
                break;
            case PISTON:
                pairs.push_back(getPiston(deviceName));
                break;
            case PROXIMITY:
                pairs.push_back(getProximity(deviceName));
        }

    return pairs;
}

void Robot::movePairMotors(const std::string& pairName, const int32_t& velocity) {
    for (const std::string& name : devicePairs.at(pairName))
        getMotor(name).move_velocity(velocity);
}

void Robot::movePairMotors(const std::string& pairName, const int32_t& velocity, const double& distance) {
    for (const std::string& name : devicePairs.at(pairName))
        getMotor(name).move_relative(distance, velocity);
}

void Robot::activatePairPiston(const std::string& pairName, bool activated) {
    for (const std::string& name : devicePairs.at(pairName))
        getPiston(name).set_value(activated);
}

void Robot::followObject(Vision& vision, const int32_t& size, const int32_t sig, Proximity& proximity, const int32_t& speed, const int64_t& minDistance, const std::vector<Motor>& leftChassis, const std::vector<Motor>& rightChassis, const int32_t& yOffset) {
    auto setMotorsSpeed = [&](const std::vector<Motor>& chassisSide, const int& speed) {
        for (const Motor& motor : chassisSide)
            motor.move_velocity(speed);
    };

    vision.setSignature(sig);
    pros::vision_object_s_t YGoal = vision.get_by_sig(size, sig);

    while (proximity.get() >= minDistance) {
        YGoal = vision.get_by_sig(size, sig);
        int yPos = YGoal.y_middle_coord - yOffset;

        while(YGoal.signature != 1) {
            setMotorsSpeed(leftChassis, speed);
            setMotorsSpeed(rightChassis, -speed);
        }

        if (yPos > 50) {
            setMotorsSpeed(leftChassis, -speed - yPos / 2);
            setMotorsSpeed(rightChassis, -speed);
        } else if (yPos < 50) {
            setMotorsSpeed(leftChassis, -speed);
            setMotorsSpeed(rightChassis, -speed - yPos / 2);
        } else {
            setMotorsSpeed(leftChassis, -speed);
            setMotorsSpeed(rightChassis, -speed);
        }

        pros::delay(100);
    }
}