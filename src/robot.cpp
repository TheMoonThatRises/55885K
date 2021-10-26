#include "robot.h"

pros::Motor robot::RB(6, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_COUNTS),
            robot::RF(7, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            robot::LB(5, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_COUNTS),
            robot::LF(8, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),

            robot::fourbarR(9, MOTOR_GEARSET_36, 1, MOTOR_ENCODER_DEGREES),
            robot::fourbarL(10, MOTOR_GEARSET_36, 0, MOTOR_ENCODER_DEGREES);

int32_t robot::wheelMaxVelocity = 100,
        robot::fourbarVelocity = 80,
        robot::wheelNormalVelocity = 50;

double robot::fourbarMaxDistance = 800;

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity) {
    robot::RB.move_velocity(rightVelocity);
    robot::RF.move_velocity(rightVelocity);
    robot::LB.move_velocity(leftVelocity);
    robot::LF.move_velocity(leftVelocity);
}

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance) {
    robot::RB.move_relative(rightDistance, rightVelocity);
    robot::RF.move_relative(rightDistance, rightVelocity);
    robot::LB.move_relative(leftDistance, leftVelocity);
    robot::LF.move_relative(leftDistance, leftVelocity);
}

void robot::moveFourbar(int32_t velocity) {
    robot::fourbarL.move_velocity(velocity);
    robot::fourbarR.move_velocity(velocity);
}

void robot::moveFourbar(int32_t velocity, double distance) {
    robot::fourbarL.move_relative(distance, velocity);
    robot::fourbarR.move_relative(distance, velocity);
}

void robot::setChassisBrake(pros::motor_brake_mode_e brakeMode) {
    robot::RB.set_brake_mode(brakeMode);
    robot::RF.set_brake_mode(brakeMode);
    robot::LB.set_brake_mode(brakeMode);
    robot::LF.set_brake_mode(brakeMode);

    robot::chassisBrake = brakeMode;
}

void robot::setFourbarBrake(pros::motor_brake_mode_e brakeMode) {
    robot::fourbarL.set_brake_mode(brakeMode);
    robot::fourbarR.set_brake_mode(brakeMode);

    robot::fourbarBrake = brakeMode;
}

bool robot::didWheelsStop() {
    if (robot::RB.is_stopped() && robot::RF.is_stopped() && robot::LB.is_stopped() && robot::LF.is_stopped()) return true;
    else return false;
}

bool robot::didFourbarStop() {
    if (robot::fourbarL.is_stopped() && robot::fourbarR.is_stopped()) return true;
    else return false;
}

void robot::initialize() {
    pros::motor_brake_mode_e chassisBrake = pros::E_MOTOR_BRAKE_COAST,
                             fourbarBrake = pros::E_MOTOR_BRAKE_COAST;

    setChassisBrake(chassisBrake);
    setFourbarBrake(fourbarBrake);
}