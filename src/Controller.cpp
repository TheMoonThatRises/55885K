#include "Controller.h"
#include "ClassHolders.h"
#include <iostream>
#include <string>


Controller::Controller(): 
    master(pros::E_CONTROLLER_MASTER)
{
    std::cout << "Successfully initialized controller" << std::endl;
}

void Controller::setControllerText(const std::string& text) {
    master.set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

void Controller::moveChassis() {
    double lVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
           rVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    
    robot.moveChassis(lVelocity / robot.chassisSensitivity, rVelocity / robot.chassisSensitivity);
}

void Controller::moveFourbar() {
    double lVelocity = 0;

    int32_t L1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1),
            L2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    if (L1) lVelocity = robot.fourbarVelocity;
    else lVelocity = -robot.fourbarVelocity;
    
    if (L1 || L2)  robot.moveFourbar(lVelocity);
    else robot.moveFourbar(0);
}

void Controller::resetFourbar() {
    setControllerText("Resetting Fourbar");
    
    
    robot.fourbarL.move_relative(-robot.fourbarL.get_position(), 100);
    robot.fourbarR.move_relative(-robot.fourbarR.get_position(), 100);

    pros::delay(1000);

    setControllerText("Fourbar Resetted");
}

void Controller::changeChassisSensitivity() {
    robot.chassisSensitivity = (robot.chassisSensitivity > 3) ? 1 : robot.chassisSensitivity + 0.5;

    setControllerText("Sensitivity < " + std::to_string(robot.chassisSensitivity));

    pros::delay(100);
}

void Controller::changeChassisSpeed() {
    robot.wheelAddedVelocity = (robot.wheelAddedVelocity >= 100) ? 0 : robot.wheelAddedVelocity + 50 ;

    setControllerText("Wheel < " + std::to_string(robot.wheelAddedVelocity));

    pros::delay(100);
}

void Controller::moveClaw() {
    robot.clawPistonValue = !robot.clawPistonValue;

    robot.clawPiston.set_value(robot.clawPistonValue);

    setControllerText("Front Claw < " + std::string(robot.clawPistonValue ? "on" : "off"));
}