#include "controller.h"
#include "robot.h"
#include "util.h"
#include <iostream>
#include <string>

pros::Controller controller::master(pros::E_CONTROLLER_MASTER);

void controller::setControllerText(std::string text) {
    controller::master.set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}

void controller::moveChassis() {
    double lVelocity = 0,
           rVelocity = 0,
           turn = 0;
    if (robot::chassisMode == robot::CHASSIS_SINGLE) {
        lVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
        rVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        turn = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    } else if (robot::chassisMode == robot::CHASSIS_TANK) {
        lVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        rVelocity = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    }
    
    robot::moveChassis(lVelocity / robot::chassisSensitivity, rVelocity / robot::chassisSensitivity, turn / robot::chassisSensitivity);
}

void controller::moveFourbar() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot::fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot::fourbarVelocity;
    
    robot::moveFourbar(velocity);
}

void controller::moveIntake() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) velocity = robot::intakeVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) velocity = -robot::intakeVelocity;

    robot::moveIntake(velocity);
}

void controller::resetFourbar() {
    robot::fourbarL.move_relative(-robot::fourbarL.get_position(), 100);
    robot::fourbarR.move_relative(-robot::fourbarR.get_position(), 100);

    controller::setControllerText("Resetting Fourbar");
}

void controller::changeChassisSensitivity() {
    robot::chassisSensitivity = (robot::chassisSensitivity > 3) ? 1 : robot::chassisSensitivity + 0.5;

    controller::setControllerText("Sensitivity < " + std::to_string(robot::chassisSensitivity));

    pros::delay(100);
}

void controller::changeChassisMode() {
    switch (robot::chassisMode) {
        case robot::CHASSIS_TANK:
            robot::chassisMode = robot::CHASSIS_SINGLE;
            break;
        case robot::CHASSIS_SINGLE:
            robot::chassisMode = robot::CHASSIS_TANK;
            break;
    }

    controller::setControllerText("Chassis < " + util::modeToString[robot::chassisMode]);

    pros::delay(200);
}

void controller::changeChassisSpeed() {
    robot::wheelAddedVelocity = (robot::wheelAddedVelocity >= 100) ? -100 : robot::wheelAddedVelocity + 50 ;

    controller::setControllerText("Wheel < " + std::to_string(robot::wheelAddedVelocity));

    pros::delay(200);
}