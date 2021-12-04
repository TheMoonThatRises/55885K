#include "Robot.h"

Robot::Robot():
    RB(6, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES),
    RF(7, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES),
    LB(5, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES),
    LF(8, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES),

    fourbarR(9, MOTOR_GEARSET_36, 1, MOTOR_ENCODER_DEGREES),
    fourbarL(10, MOTOR_GEARSET_36, 0, MOTOR_ENCODER_DEGREES),

    backFourbarR(12, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES),
    backFourbarL(13, MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES),

    fourbarVelocity(80),
    wheelAddedVelocity(0),
    chassisVelocity(90),
    backGoalVelocity(80),

    chassisSensitivity(1),
    fourbarMaxDistance(800),

    chassisMode(CHASSIS_TANK),

    fourbarBrake(pros::E_MOTOR_BRAKE_HOLD),
    chassisBrake(pros::E_MOTOR_BRAKE_BRAKE)
{
    std::cout << "Successfully initialized Robot" << std::endl;
}

void Robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double turn) const {
    if (leftVelocity || rightVelocity || turn != 0) {
        leftVelocity += (leftVelocity > 0) ? wheelAddedVelocity : -wheelAddedVelocity;
        rightVelocity += (rightVelocity > 0) ? wheelAddedVelocity : -wheelAddedVelocity;
        turn += (turn > 0) ? wheelAddedVelocity : -wheelAddedVelocity;

        RB.move_velocity(rightVelocity - turn);
        RF.move_velocity(rightVelocity - turn);
        LB.move_velocity(leftVelocity + turn);
        LF.move_velocity(leftVelocity + turn);
    } else {
        RB.move_velocity(0);
        RF.move_velocity(0);
        LB.move_velocity(0);
        LF.move_velocity(0);
    }
}

void Robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance, double turn) const {
    RB.move_relative(rightDistance - turn, rightVelocity);
    RF.move_relative(rightDistance - turn, rightVelocity);
    LB.move_relative(leftDistance + turn, leftVelocity);
    LF.move_relative(leftDistance + turn, leftVelocity);
}

void Robot::moveFourbar(int32_t velocity) const {
    if (velocity > 0) {
        fourbarL.move_velocity(velocity);
        fourbarR.move_velocity(velocity);
    } else if (velocity < 0) {
        velocity = -fourbarVelocity;

        fourbarL.move_velocity(velocity);
        fourbarR.move_velocity(velocity);
    }
    else {
        fourbarL.move_velocity(0);
        fourbarR.move_velocity(0);
    }
}

void Robot::moveFourbar(const int32_t velocity, const double distance) const {
    fourbarL.move_relative(distance, velocity);
    fourbarR.move_relative(distance, velocity);
}

void Robot::moveBackFourbar(int32_t velocity) const {
    if (velocity > 0) {
        backFourbarL.move_velocity(velocity);
        backFourbarR.move_velocity(velocity);
    } else if (velocity < 0) {
        velocity = -fourbarVelocity;

        backFourbarL.move_velocity(velocity);
        backFourbarR.move_velocity(velocity);
    }
    else {
        backFourbarL.move_velocity(0);
        backFourbarR.move_velocity(0);
    }
}

void Robot::moveBackFourbar(const int32_t velocity, const double distance) const {
    backFourbarL.move_relative(distance, velocity);
    backFourbarR.move_relative(distance, velocity);
}

void Robot::setChassisBrake(const pros::motor_brake_mode_e brakeMode) {
    RB.set_brake_mode(brakeMode);
    RF.set_brake_mode(brakeMode);
    LB.set_brake_mode(brakeMode);
    LF.set_brake_mode(brakeMode);

    chassisBrake = brakeMode;
}

void Robot::setFourbarBrake(const pros::motor_brake_mode_e brakeMode) const {
    fourbarL.set_brake_mode(brakeMode);
    fourbarR.set_brake_mode(brakeMode);
}

bool Robot::didWheelsStop() const {
    if (RB.is_stopped() && RF.is_stopped() && LB.is_stopped() && LF.is_stopped()) return true;
    else return false;
}

bool Robot::checkFourbar() const {
    return fourbarL.get_position() == fourbarR.get_position();
}

void Robot::initialize() {
    setChassisBrake(chassisBrake);
    setFourbarBrake(fourbarBrake);
}