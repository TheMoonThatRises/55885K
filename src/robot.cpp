#include "robot.h"

pros::Motor robot::RB(6, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES),
            robot::RF(7, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES),
            robot::LB(5, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES),
            robot::LF(8, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES),

            robot::fourbarR(9, MOTOR_GEARSET_36, 1, MOTOR_ENCODER_DEGREES),
            robot::fourbarL(10, MOTOR_GEARSET_36, 0, MOTOR_ENCODER_DEGREES),

            robot::ringMotor(12, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES);

int32_t robot::fourbarVelocity = 80,
        robot::wheelAddedVelocity = 0,
        robot::intakeVelocity = 85,
        robot::chassisVelocity = 90;

double robot::chassisSensitivity = 1,
       robot::fourbarMaxDistance = 800;

int robot::chassisMode = robot::CHASSIS_TANK;

pros::motor_brake_mode_e robot::fourbarBrake = pros::E_MOTOR_BRAKE_HOLD,
                         robot::chassisBrake = pros::E_MOTOR_BRAKE_BRAKE;

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double turn) {
    if (leftVelocity || rightVelocity || turn) {
        leftVelocity += (leftVelocity > 0) ? robot::wheelAddedVelocity : -robot::wheelAddedVelocity;
        rightVelocity += (rightVelocity > 0) ? robot::wheelAddedVelocity : -robot::wheelAddedVelocity;
        turn += (turn > 0) ? robot::wheelAddedVelocity : -robot::wheelAddedVelocity;

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

void robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance, double turn) {
    robot::RB.move_relative(rightDistance - turn, rightVelocity);
    robot::RF.move_relative(rightDistance - turn, rightVelocity);
    robot::LB.move_relative(leftDistance + turn, leftVelocity);
    robot::LF.move_relative(leftDistance + turn, leftVelocity);
}

void robot::moveFourbar(int32_t velocity) {
    if (velocity > 0) {
        robot::fourbarL.move_velocity(velocity);
        robot::fourbarR.move_velocity(velocity);
    } else if (velocity < 0) {
        velocity = -robot::fourbarVelocity;

        robot::fourbarL.move_velocity(velocity);
         robot::fourbarR.move_velocity(velocity);
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

void robot::moveIntake(int32_t velocity){
    if (velocity != 0) robot::ringMotor.move_velocity(velocity);
    else robot::ringMotor.move_velocity(0);
}

void robot::moveIntake(int32_t velocity, double distance) {
    robot::ringMotor.move_relative(distance, velocity);
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

    robot::ringMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}