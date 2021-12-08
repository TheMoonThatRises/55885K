#include "main.h"
#include "ClassHolders.h"
#include <iostream>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	std::cout << "Initializing..." << std::endl;
	robot.initialize();
}

/**
 * Runs while the Robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either Autonomous or opcontrol. When
 * the Robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before Autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an Autonomous selector
 * on the LCD.
 *
 * This task will exit when the Robot is enabled and Autonomous or opcontrol
 * starts.
 */


void competition_initialize() {
    auton.selectAuton();
}

/**
 * Runs the user Autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the Robot is enabled via
 * the Field Management System or the VEX Competition Switch in the Autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the Robot is disabled or communications is lost, the Autonomous task
 * will be stopped. Re-enabling the Robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    Autonomous::loadRunString(util.Auton[util.auton]);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the Robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the Robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the Robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	std::cout << "Running opcontrol function." << std::endl;

    while (true) {
        controller.moveChassis();
        controller.moveFourbar();

        if (controller.master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) controller.resetFourbar();
        if (controller.master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) controller.moveClaw();
        if (controller.master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) controller.changeChassisSensitivity();
        if (controller.master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) controller.changeChassisSpeed();

		pros::delay(20);
	}
}