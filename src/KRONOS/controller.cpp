#include "controller.hpp"

using namespace KRONOS;

Controller::Controller(KRONOS::Robot& robot):
    pros::Controller(pros::E_CONTROLLER_MASTER),
    robot(robot)
{

}

template<class T>
Controller& Controller::link(std::map<T, std::vector<std::string>>& linker, const T& control, const std::vector<std::string>& callsigns) {
    linker.insert({control, callsigns});

    return *this;
}

void Controller::setControllerText(const std::string& text) {
    set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

Controller& Controller::linkDigital(const pros::controller_digital_e_t& control, const std::string& callsign, const bool& isPair) {
    return link(digitalLink, control, isPair ? robot.getMotorPair(callsign) : std::vector<std::string> {callsign});
}

Controller& Controller::linkAnalog(const pros::controller_analog_e_t& control, const std::string& callsign, const bool& isPair) {
    return link(analogLink, control, std::vector<std::string> {callsign});
}

void Controller::listener() {
    for (const auto& [controlType, devicePair] : digitalLink)
        if (get_digital(controlType));


    for (const auto& [controlType, devicePair] : analogLink)
        if (get_analog(controlType))
            for (std::string callsign: devicePair)
                switch (robot.getDeviceType(callsign)) {
                    case PISTON:
                        robot.getPiston(callsign).set_value(get_analog(controlType));
                        break;
                    case MOTOR:
                        robot.getMotor(callsign).move_velocity(get_analog(controlType));
                        break;
                }
}