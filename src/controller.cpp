#include "controller.h"
#include "robot.h"

pros::Controller controller::master(pros::E_CONTROLLER_MASTER);

void controller::moveChassis() {
    double fbJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    double stsJoystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
}

void controller::moveFourbar() {
    int32_t velocity = 0;

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) velocity = robot::fourbarVelocity;
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) velocity = -robot::fourbarVelocity;

    if (robot::fourbarL.get_position() > 0 && robot::fourbarL.get_position() < robot::fourbarMaxDistance) robot::fourbarL.move_velocity(velocity);
    if (robot::fourbarR.get_position() > 0 && robot::fourbarR.get_position() < robot::fourbarMaxDistance) robot::fourbarR.move_velocity(velocity);
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
}