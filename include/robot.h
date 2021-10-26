#include "main.h"

class robot {
    public:
        static pros::Motor RB, RF, LB, LF, fourbarR, fourbarL;
        static pros::motor_brake_mode_e chassisBrake, fourbarBrake;
        static int32_t wheelMaxVelocity, fourbarVelocity, wheelNormalVelocity;
        static double fourbarMaxDistance;
        static void moveChassis(int32_t leftVelocity, int32_t rightVelocity);
        static void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance);
        static void moveFourbar(int32_t velocity);
        static void moveFourbar(int32_t velocity, double distance);
        static void setChassisBrake(pros::motor_brake_mode_e brakeMode);
        static void setFourbarBrake(pros::motor_brake_mode_e brakeMode);
        static bool didWheelsStop();
        static bool didFourbarStop();
        static void initialize();
};