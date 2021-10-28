#include "controller.h"
#include "robot.h"
#include <iostream>

pros::Controller controller::master(pros::E_CONTROLLER_MASTER);

void controller::moveChassis() {
    double fbJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    double stsJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    if (fbJoystick != 0) {
        robot::wheelAdditionalParallelSpeed = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        robot::wheelAditionTurnSpeed = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        robot::moveChassis(fbJoystick, fbJoystick, stsJoystick);
    } else robot::moveChassis(0, 0, 0);
}

void controller::moveFourbar() {
    double velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot::fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot::fourbarVelocity;
    
    robot::moveFourbar(velocity, false);

    std::cout << robot::fourbarL.is_stopped() << robot::fourbarR.is_stopped() << std::endl;
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

    std::cout << "Switch chassis brake mode to " << robot::chassisBrake << std::endl;
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

    std::cout << "Switch fourbar brake mode to " << robot::fourbarBrake << std::endl;
}