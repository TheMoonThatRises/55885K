#include "Controller.h"
#include "ClassHolders.h"
#include <iostream>
#include <string>


Controller::Controller(): 
    master(pros::E_CONTROLLER_MASTER)
{
    std::cout << "Successfully initialized controller" << std::endl;
}

void Controller::setControllerText(const std::string text) {
    master.set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

void Controller::moveChassis() {
    double lVelocity = 0,
           rVelocity = 0,
           turn = 0;
    if (robot.chassisMode == robot.CHASSIS_SINGLE) {
        lVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
        rVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        turn = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    } else if (robot.chassisMode == robot.CHASSIS_TANK) {
        lVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        rVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    }
    
    robot.moveChassis(lVelocity / robot.chassisSensitivity, rVelocity / robot.chassisSensitivity, turn / robot.chassisSensitivity);
}

void Controller::moveFourbar() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot.fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot.fourbarVelocity;
    
    robot.moveFourbar(velocity);
}

void Controller::moveBackGoal() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) velocity = robot.backGoalVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) velocity = -robot.backGoalVelocity;

    robot.moveBackGoal(velocity);
}

void Controller::resetFourbar() {
    Controller::setControllerText("Resetting Fourbar");

    robot.fourbarL.move_relative(-robot.fourbarL.get_position(), 100);
    robot.fourbarR.move_relative(-robot.fourbarR.get_position(), 100);

    pros::delay(2000);

    Controller::setControllerText("Fourbar Resetted");
}

void Controller::changeChassisSensitivity() {
    robot.chassisSensitivity = (robot.chassisSensitivity > 3) ? 1 : robot.chassisSensitivity + 0.5;

    Controller::setControllerText("Sensitivity < " + std::to_string(robot.chassisSensitivity));

    pros::delay(100);
}

void Controller::changeChassisMode() {
    if (robot.chassisMode == robot.CHASSIS_TANK) robot.chassisMode = robot.CHASSIS_SINGLE;
    else robot.chassisMode = robot.CHASSIS_TANK;

    Controller::setControllerText("Chassis < " + util.modeToString[robot.chassisMode]);

    pros::delay(200);
}

void Controller::changeChassisSpeed() {
    robot.wheelAddedVelocity = (robot.wheelAddedVelocity >= 100) ? -100 : robot.wheelAddedVelocity + 50 ;

    Controller::setControllerText("Wheel < " + std::to_string(robot.wheelAddedVelocity));

    pros::delay(200);
}