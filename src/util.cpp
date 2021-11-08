#include "util.h"
#include "robot.h"
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

void util::calibrateFourbar() {

}

void util::autonomous() {

}