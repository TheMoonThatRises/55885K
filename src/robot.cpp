#include "robot.h"

pros::Motor robot::RB(6, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_COUNTS),
            robot::RF(7, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_COUNTS),
            robot::LB(5, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),
            robot::LF(8, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_COUNTS),

            robot::fourbarR(9, MOTOR_GEARSET_36, 1, MOTOR_ENCODER_DEGREES),
            robot::fourbarL(10, MOTOR_GEARSET_36, 0, MOTOR_ENCODER_DEGREES);

int32_t robot::wheelMaxVelocity = 0,
        robot::fourbarVelocity = 80,
        robot::wheelNormalVelocity = 0;

double robot::fourbarMaxDistance = 800;

pros::motor_brake_mode_e robot::fourbarBrake = pros::E_MOTOR_BRAKE_HOLD,
                         robot::chassisBrake = pros::E_MOTOR_BRAKE_COAST;

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double turn) {
    if (leftVelocity || rightVelocity || turn) {
        leftVelocity += (leftVelocity > 0) ? robot::wheelNormalVelocity : -robot::wheelNormalVelocity;
        rightVelocity += (rightVelocity > 0) ? robot::wheelNormalVelocity : -robot::wheelNormalVelocity;
        turn += (turn > 0) ? robot::wheelNormalVelocity : -robot::wheelNormalVelocity;

        robot::RB.move_velocity(rightVelocity - turn);
        robot::RF.move_velocity(rightVelocity - turn);
        robot::LB.move_velocity(leftVelocity + turn);
        robot::LF.move_velocity(leftVelocity + turn);
    } else {
        robot::RB.move_velocity(0);
        robot::RF.move_velocity(0);
        robot::LB.move_velocity(0);
        robot::LF.move_velocity(0);
    }
}

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance) {
    robot::RB.move_relative(rightDistance, rightVelocity);
    robot::RF.move_relative(rightDistance, rightVelocity);
    robot::LB.move_relative(leftDistance, leftVelocity);
    robot::LF.move_relative(leftDistance, leftVelocity);
}

void robot::moveFourbar(int32_t velocity, bool override) {
    if (velocity > 0) {
        if (robot::fourbarL.get_position() < robot::fourbarMaxDistance || override) robot::fourbarL.move_velocity(velocity);
        if (robot::fourbarR.get_position() < robot::fourbarMaxDistance || override) robot::fourbarR.move_velocity(velocity);
    } else if (velocity < 0) {
        velocity = -robot::fourbarVelocity;

        if (robot::fourbarL.get_position() > 0 || override) robot::fourbarL.move_velocity(velocity);
        if (robot::fourbarR.get_position() > 0 || override) robot::fourbarR.move_velocity(velocity);
    }
    else {
        robot::fourbarL.move_velocity(0);
        robot::fourbarR.move_velocity(0);
    }
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

bool robot::isFourbarMoving() {
    if (robot::fourbarL.get_target_velocity() - robot::fourbarL.get_actual_velocity() < robot::fourbarL.get_target_velocity() / 2 && robot::fourbarR.get_target_velocity() - robot::fourbarR.get_actual_velocity() < robot::fourbarR.get_target_velocity() / 2) return true;
    else return false;
} // NOT WORKING

bool robot::didWheelsStop() {
    if (robot::RB.is_stopped() && robot::RF.is_stopped() && robot::LB.is_stopped() && robot::LF.is_stopped()) return true;
    else return false;
}

bool robot::checkFourbar() {
    return robot::fourbarL.get_position() == robot::fourbarR.get_position();
}

void robot::initialize() {
    setChassisBrake(chassisBrake);
    setFourbarBrake(fourbarBrake);
}