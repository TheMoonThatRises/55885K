#include "util.h"
#include "robot.h"
#include "controller.h"
#include <string>

std::string util::brakeToString[] = {"Coast", "Brake", "Hold"};
std::string util::modeToString[] = {"Tank", "Single"};

void util::testWheels() {
    robot::moveChassis(robot::wheelNormalVelocity, robot::wheelNormalVelocity, 5000, 5000, 0);

    pros::delay(2000);

    robot::moveChassis(robot::wheelNormalVelocity, robot::wheelNormalVelocity, -5000, -5000, 0);
}

void util::testFourbar() {
    robot::moveFourbar(robot::fourbarVelocity, robot::fourbarMaxDistance);

    pros::delay(2000);

    robot::moveFourbar(robot::fourbarVelocity, -robot::fourbarMaxDistance);
}

void util::testAll() {
    util::testWheels();

    while (!robot::didWheelsStop()) {
        pros::delay(20);
    }

    util::testFourbar();
}

bool util::checkOverheat() {
    if (robot::RB.is_over_temp() || robot::RB.is_over_temp() || robot::LF.is_over_temp() || robot::LB.is_over_temp() || robot::fourbarL.is_over_temp() || robot::fourbarR.is_over_temp() || robot::ringMotor.is_over_temp()) return true;
    else return false;
}

void util::calibrateFourbar() {

}