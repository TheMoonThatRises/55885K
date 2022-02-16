#include "controller.hpp"

using namespace KRONOS;

Controller::Controller(KRONOS::Robot& robot):
    pros::Controller(pros::E_CONTROLLER_MASTER),
    robot(robot)
{

}

template<class T>
Controller& Controller::link(std::map<T, std::map<std::string, function>>& linker, const T& control, const function action, const function idle) {
    linker.insert({control, {{"action", action}, {"idle", idle}}});

    return *this;
}

void Controller::setControllerText(const std::string& text) {
    set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

Controller& Controller::linkDigital(const pros::controller_digital_e_t& control, const function action, const function idle) {
    return link(digitalLink, control, action, idle);
}

Controller& Controller::linkAnalog(const pros::controller_analog_e_t& control, const function action) {
    return link(analogLink, control, action, nullptr);
}

void Controller::listener() {
    std::vector<int> activeButtons;

    for (const Device<Motor>& motor : robot.getMotorVector())
        if (motor.device.is_over_temp()) {
            rumble("_._._._.");
            break;
        }

    for (const auto& [controlType, type] : digitalLink)
        if (get_digital(controlType)) {
            activeButtons.push_back(controlType);
            type.at("action")();
        }


    for (const auto& [controlType, type] : analogLink)
        type.at("action")();
            
    for (const auto& [controlType, type] : digitalLink) 
        if (type.at("idle") != nullptr && std::find(activeButtons.begin(), activeButtons.end(), controlType + ((controlType % 2 == 0) ? 1 : -1)) == activeButtons.end() && std::find(activeButtons.begin(), activeButtons.end(), controlType) == activeButtons.end())
            type.at("idle")();
}