#include "util.h"
#include "robot.h"

void util::testWheels() {
    robot::tankRelative(robot::wheelNormalVelocity, robot::wheelNormalVelocity, 5000, 5000);

    pros::delay(2000);

    robot::tankRelative(robot::wheelNormalVelocity, robot::wheelNormalVelocity, -5000, -5000);

}

void util::testFourbar() {
    robot::moveFourbar(robot::fourbarMaxVelocity, robot::fourbarMaxDistance);

    pros::delay(2000);

    robot::moveFourbar(robot::fourbarMaxVelocity, -robot::fourbarMaxDistance);


}

void util::testAll() {
    util::testWheels();

    while (!robot::didWheelsStop()) {
        pros::delay(20);
    }

    util::testFourbar();
}