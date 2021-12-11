#include "Robot.h"

Robot::Robot():
    RB(6, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES),
    RF(7, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES),
    LB(5, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES),
    LF(8, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES),

    fourbarR(9, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES),
    fourbarL(10, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES),

    backLift(12, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES),

    clawPiston('A'),

    fourbarVelocity(80),
    wheelAddedVelocity(0),

    chassisSensitivity(1),

    fourbarBrake(pros::E_MOTOR_BRAKE_HOLD),
    chassisBrake(pros::E_MOTOR_BRAKE_BRAKE)
{
    std::cout << "Successfully initialized Robot" << std::endl;
}

void Robot::moveChassis(int32_t leftVelocity, int32_t rightVelocity) const {
    if (leftVelocity || rightVelocity) {
        leftVelocity += (leftVelocity != 0) ? ((leftVelocity > 0) ? wheelAddedVelocity : -wheelAddedVelocity) : 0;
        rightVelocity += (rightVelocity != 0) ? ((rightVelocity > 0) ? wheelAddedVelocity : -wheelAddedVelocity) : 0;

        RB.move_velocity(rightVelocity);
        RF.move_velocity(rightVelocity);
        LB.move_velocity(leftVelocity);
        LF.move_velocity(leftVelocity);
    } else {
        RB.move_velocity(0);
        RF.move_velocity(0);
        LB.move_velocity(0);
        LF.move_velocity(0);
    }
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

void Robot::moveBackLift(const int32_t velocity) {
    backLift.move_velocity(velocity);
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

void Robot::initialize() {
    setChassisBrake(chassisBrake);
    setFourbarBrake(fourbarBrake);
    backLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    clawPiston.set_value(false);
}