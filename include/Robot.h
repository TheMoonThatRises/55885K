#include "main.h"
#include "KRONOS/kronos.h"
#include <string>


#ifndef _ROBOT_H_
#define _ROBOT_H_

class Robot {
    public:
        Robot();

        const KRONOS::Motor RB, RF, LB, LF, fourbarR, fourbarL, backLift;
        KRONOS::Piston clawPiston;
        pros::motor_brake_mode_e chassisBrake, fourbarBrake;
        int32_t fourbarVelocity, wheelAddedVelocity;
        double fourbarMaxDistance, chassisSensitivity;
        void moveChassis(int32_t leftVelocity, int32_t rightVelocity) const;
        void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance) const;
        void moveFourbar(int32_t velocity) const;
        void moveFourbar(int32_t velocity, double distance) const;
        void moveBackLift(int32_t velocity);
        void moveBackLift(int32_t velocity, double distance);
        void setChassisBrake(pros::motor_brake_mode_e brakeMode);
        void setFourbarBrake(pros::motor_brake_mode_e brakeMode) const;
        bool didWheelsStop() const;
        void initialize();
};

#endif