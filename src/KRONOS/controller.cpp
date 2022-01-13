#include "controller.hpp"

using namespace KRONOS;

Controller::Controller(KRONOS::Robot& robot):
    pros::Controller(pros::E_CONTROLLER_MASTER),
    robot(robot)
{

}

template<class T>
Controller& Controller::link(std::map<T, function<T>>& linker, const T& control, const function<T> action) {
    linker.insert({control, action});

    return *this;
}

void Controller::setControllerText(const std::string& text) {
    set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

Controller& Controller::linkDigital(const pros::controller_digital_e_t& control, const function<pros::controller_digital_e_t> action) {
    return link(digitalLink, control, action);
}

Controller& Controller::linkAnalog(const pros::controller_analog_e_t& control, const function<pros::controller_analog_e_t> action) {
    return link(analogLink, control, action);
}

void Controller::listener() {
    for (const auto& [controlType, action] : digitalLink)
        if (get_digital(controlType))
            action(controlType);


    for (const auto& [controlType, action] : analogLink)
        if (get_analog(controlType))
            action(controlType);
            
}