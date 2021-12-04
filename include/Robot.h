#include "main.h"
#include <string>


#ifndef _ROBOT_H_
#define _ROBOT_H_

class Robot {
    public:
        Robot();

        const int CHASSIS_TANK = 0,
                  CHASSIS_SINGLE = 1;

        const pros::Motor RB, RF, LB, LF, fourbarR, fourbarL, backFourbarR, backFourbarL;
        pros::motor_brake_mode_e chassisBrake, fourbarBrake;
        int32_t fourbarVelocity, wheelAddedVelocity, chassisVelocity, backGoalVelocity;
        double fourbarMaxDistance, chassisSensitivity;
        int chassisMode;
        void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double turn) const;
        void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance, double turn) const;
        void moveFourbar(int32_t velocity) const;
        void moveFourbar(int32_t velocity, double distance) const;
        void moveBackFourbar(int32_t velocity) const;
        void moveBackFourbar(int32_t velocity, double distance) const;
        void setChassisBrake(pros::motor_brake_mode_e brakeMode);
        void setFourbarBrake(pros::motor_brake_mode_e brakeMode) const;
        bool didWheelsStop() const;
        bool checkFourbar() const;
        void initialize();
};

#endif