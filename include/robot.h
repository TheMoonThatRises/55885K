#include "main.h"
#include <string>

class robot {
    public:
        static const int CHASSIS_TANK = 0,
                         CHASSIS_SINGLE = 1;

        static pros::Motor RB, RF, LB, LF, fourbarR, fourbarL, ringMotor;
        static pros::motor_brake_mode_e chassisBrake, fourbarBrake;
        static int32_t fourbarVelocity, wheelNormalVelocity, intakeVelocity, chassisSensitivity;
        static double fourbarMaxDistance;
        static int chassisMode;
        static void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double turn);
        static void moveChassis(int32_t leftVelocity, int32_t rightVelocity, double leftDistance, double rightDistance, double turn);
        static void moveFourbar(int32_t velocity, bool override);
        static void moveFourbar(int32_t velocity, double distance);
        static void moveIntake(int32_t velocity);
        static void setChassisBrake(pros::motor_brake_mode_e brakeMode);
        static void setFourbarBrake(pros::motor_brake_mode_e brakeMode);
        static bool isFourbarMoving();
        static bool didWheelsStop();
        static bool checkFourbar();
        static void initialize();
};