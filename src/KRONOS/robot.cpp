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

std::vector<Motor> Robot::getMotorPairs(const std::string& name) {
    std::vector<Motor> pairs;

    for (const std::string& deviceName : devicePairs.at(name))
        pairs.push_back(getMotor(deviceName));

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

void Robot::followObject(Vision& vision, const int32_t& size, const int32_t sig, Proximity& proximity, const int32_t& speed, const int64_t& minDistance, const std::vector<Motor>& leftChassis, const std::vector<Motor>& rightChassis, const int32_t& yOffset, const int& turnDir) {
    auto setMotorsSpeed = [&](const std::vector<Motor>& chassisSide, const int& speed) {
        for (const Motor& motor : chassisSide)
            motor.move_velocity(speed);
    };

    vision.setSignature(sig);
    pros::vision_object_s_t YGoal = vision.get_by_sig(size, sig);

    while (true) {
        int distance = proximity.get();
        YGoal = vision.get_by_sig(size, sig);
        int xPos = YGoal.x_middle_coord - yOffset;

        if (distance == PROS_ERR)
            std::cout << "Distance error -- errno" << errno << std::endl;
        else if (distance < minDistance && distance > 0 && YGoal.signature == 1)
            break;

        while(YGoal.signature != 1 || (xPos < 40 && xPos > 60)) {
            setMotorsSpeed(leftChassis, -speed * turnDir);
            setMotorsSpeed(rightChassis, speed * turnDir);
            YGoal = vision.get_by_sig(size, sig);
        }

        // if (xPos > 50) {
        //     setMotorsSpeed(leftChassis, -speed - xPos / 2);
        //     setMotorsSpeed(rightChassis, -speed);
        // } else if (xPos < 50) {
        //     setMotorsSpeed(leftChassis, -speed);
        //     setMotorsSpeed(rightChassis, -speed - xPos / 2);
        // } else {
            setMotorsSpeed(leftChassis, -speed);
            setMotorsSpeed(rightChassis, -speed);
        // }

        pros::delay(20);
    }
}