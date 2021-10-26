#include "util.h"
#include "robot.h"

void util::testWheels() {
    robot::moveChassis(robot::wheelNormalVelocity, robot::wheelNormalVelocity, 5000, 5000);

    pros::delay(2000);

    robot::moveChassis(robot::wheelNormalVelocity, robot::wheelNormalVelocity, -5000, -5000);
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