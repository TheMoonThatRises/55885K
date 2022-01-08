#include "KRONOS/kronos.h"
#include "KRONOS/helpers.h"

KRONOS::Robot robot;
KRONOS::Controller controller(robot);
KRONOS::Autonomous auton(robot, controller);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	std::cout << "Initializing..." << std::endl;
	robot
		.addMotor(KRONOS::Device(KRONOS::Motor(8, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "frontLeftTank", "fl"))
		.addMotor(KRONOS::Device(KRONOS::Motor(7, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "frontRightTank", "fr"))
		.addMotor(KRONOS::Device(KRONOS::Motor(5, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "backLeftTank", "bl"))
		.addMotor(KRONOS::Device(KRONOS::Motor(6, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "backRightTank", "br"))

		.addMotor(KRONOS::Device(KRONOS::Motor(10, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "leftFourbar", "lf"))
		.addMotor(KRONOS::Device(KRONOS::Motor(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "rightFourbar", "rf"))

		.addMotor(KRONOS::Device(KRONOS::Motor(NULL, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backDrag", "bd"))
		.addMotor(KRONOS::Device(KRONOS::Motor(NULL, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backDrag2", "d2"))

		.addPiston(KRONOS::Device(KRONOS::Piston('a'), "claw", "cl"))

		.addButton("select", pros::ADIDigitalIn('b'))
		.addButton("lock", pros::ADIDigitalIn('c'));

	robot
		.pairDevices({"fl", "bl"}, "leftTank")
		.pairDevices({"fr", "br"}, "rightTank")
		.pairDevices({"lf", "rf"}, "fourbar");

	controller
		.linkAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y, "rightTank", true)
		.linkAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y, "leftTank", true);
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
    auton.runAuton();
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
		controller.listener();

		pros::delay(20);
	}
}