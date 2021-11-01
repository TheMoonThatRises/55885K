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
    double fbJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    double stsJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    
    robot::moveChassis(fbJoystick, fbJoystick, stsJoystick);
}

void controller::moveFourbar() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot::fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot::fourbarVelocity;
    
    robot::moveFourbar(velocity, false);
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

void controller::changeChassisSpeed() {
    robot::wheelNormalVelocity = (robot::wheelNormalVelocity >= 100) ? -100 : robot::wheelNormalVelocity + 50 ;

    controller::setControllerText("Wheel < " + std::to_string(robot::wheelNormalVelocity));

    pros::delay(200);
}