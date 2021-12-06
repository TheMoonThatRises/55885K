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
    double lVelocity = 0,
           rVelocity = 0;

    int32_t L1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1),
            L2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2),
            R1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1),
            R2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

    if (L1) lVelocity = robot.fourbarVelocity;
    else lVelocity = -robot.fourbarVelocity;

    if (R1) rVelocity = robot.fourbarVelocity;
    else rVelocity = -robot.fourbarVelocity;
    
    if (L1 || L2)  robot.moveFourbar(robot.fourbarL, robot.fourbarR, lVelocity);
    else robot.moveFourbar(robot.fourbarL, robot.fourbarR, 0);

    if (R1 || R2) robot.moveFourbar(robot.backFourbarL, robot.backFourbarR, rVelocity);
    else robot.moveFourbar(robot.backFourbarL, robot.backFourbarR, 0);
}

void Controller::resetFourbar(pros::Motor fourbarL, pros::Motor fourbarR) {
    Controller::setControllerText("Resetting Fourbar");
    
    
    fourbarL.move_relative(-robot.fourbarL.get_position(), 100);
    fourbarR.move_relative(-robot.fourbarR.get_position(), 100);

    pros::delay(1000);

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

    pros::delay(100);
}

void Controller::changeChassisSpeed() {
    robot.wheelAddedVelocity = (robot.wheelAddedVelocity >= 100) ? 0 : robot.wheelAddedVelocity + 50 ;

    Controller::setControllerText("Wheel < " + std::to_string(robot.wheelAddedVelocity));

    pros::delay(100);
}