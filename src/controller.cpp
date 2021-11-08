#include "controller.h"
#include "robot.h"
#include "util.h"
#include <iostream>
#include <string>

pros::Controller controller::master(pros::E_CONTROLLER_MASTER);

void controller::setControllerText(std::string text) {
    controller::master.set_text(0, 0, text + "      ");

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
    
    robot::moveChassis(lVelocity, rVelocity, turn);

    controller::setControllerText("Chassis < " + util::modeToString[robot::chassisMode]);

    pros::delay(200);
}

void controller::moveFourbar() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot::fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot::fourbarVelocity;
    
    robot::moveFourbar(velocity, false);
}

void controller::moveIntake() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) velocity = robot::intakeVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) velocity = -robot::intakeVelocity;

    robot::moveIntake(velocity);
}

void controller::changeChassisBrake() {
    switch (robot::chassisBrake) {
        case pros::E_MOTOR_BRAKE_COAST:
            robot::setChassisBrake(pros::E_MOTOR_BRAKE_HOLD);
            break;
        case pros::E_MOTOR_BRAKE_HOLD:
            robot::setChassisBrake(pros::E_MOTOR_BRAKE_BRAKE);
            break;
        case pros::E_MOTOR_BRAKE_BRAKE:
            robot::setChassisBrake(pros::E_MOTOR_BRAKE_COAST);
            break;
    }

    controller::setControllerText("Chassis < " + util::brakeToString[robot::chassisBrake]);

    pros::delay(200);
}

void controller::changeFourbarBrake() {
    switch (robot::fourbarBrake) {
        case pros::E_MOTOR_BRAKE_COAST:
            robot::setFourbarBrake(pros::E_MOTOR_BRAKE_HOLD);
            break;
        case pros::E_MOTOR_BRAKE_HOLD:
            robot::setFourbarBrake(pros::E_MOTOR_BRAKE_BRAKE);
            break;
        case pros::E_MOTOR_BRAKE_BRAKE:
            robot::setFourbarBrake(pros::E_MOTOR_BRAKE_COAST);
            break;
    }

    controller::setControllerText("Fourbar < " + util::brakeToString[robot::fourbarBrake]);

    pros::delay(200);
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
}

void controller::changeChassisSpeed() {
    robot::wheelNormalVelocity = (robot::wheelNormalVelocity >= 100) ? -100 : robot::wheelNormalVelocity + 50 ;

    controller::setControllerText("Wheel < " + std::to_string(robot::wheelNormalVelocity));

    pros::delay(200);
}