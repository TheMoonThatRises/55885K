#include "controller.h"
#include "robot.h"

pros::Controller controller::master(pros::E_CONTROLLER_MASTER);

void controller::moveChassis() {

}

void controller::moveFourbar() {

}

void controller::setChassisBrake() {
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

void controller::setFourbarBrake() {
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