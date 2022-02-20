#include "KRONOS/kronos.h"
#include "KRONOS/helpers.h"

#define YGOAL 1

KRONOS::Robot robot(0, 1);
KRONOS::Controller controller(robot);
KRONOS::Autonomous auton(robot, controller, {"fl200_fr200_bl200_br200_lf200_rf200_ln700\nfl200_fr200_bl200_br200_lf-200_rf-200_ln800\ncl\nfl-100_fr-100_bl-100_br-100_ln2400\ncl", "fl200_fr200_bl200_br200_lf200_rf200_kl200_ln700\nfl200_fr200_bl200_br200_lf-200_rf-200_kl200_ln800\ncl\nfl-200_fr-200_bl-200_br-200_kl200_lf20_rf20_ln1000\ntk_vs_0_1_px_50_110_leftTank_rightTank_80_-1\nkl-200_ln1000\nfl200_fr100_bl200_br100_ln2000", "tk_vs_0_1_px_50_110_leftTank_rightTank_30_-1", ""}, {"midGoals", "allGoals", "blueGoal", "None"}, 0);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	std::cout << "Initializing..." << std::endl;
	robot
		// Adding chassis motors
		.addMotor(KRONOS::Device(KRONOS::Motor(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_BRAKE), "frontRightTank", "fr"))
		.addMotor(KRONOS::Device(KRONOS::Motor(7, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_BRAKE), "frontLeftTank", "fl"))
		.addMotor(KRONOS::Device(KRONOS::Motor(5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_BRAKE), "backRightTank", "br"))
		.addMotor(KRONOS::Device(KRONOS::Motor(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_BRAKE), "backLeftTank", "bl"))

		// Adding fourbar motors
		.addMotor(KRONOS::Device(KRONOS::Motor(10, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "leftFourbar", "lf"))
		.addMotor(KRONOS::Device(KRONOS::Motor(9, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "rightFourbar", "rf"))

		// Adding back motors
		.addMotor(KRONOS::Device(KRONOS::Motor(11, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backDrag", "bd"))
		.addMotor(KRONOS::Device(KRONOS::Motor(12, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backLift", "kl"))

		// Adding piston claw
		.addPiston(KRONOS::Device(KRONOS::Piston('a'), "claw", "cl"))

		// Adding buttons
		.addButton("select", pros::ADIDigitalIn('b'))
		.addButton("lock", pros::ADIDigitalIn('c'))

		.addVision(KRONOS::Device(KRONOS::Vision(17), "vision", "vs"))
		.addProximity(KRONOS::Device(KRONOS::Proximity(4), "proximity", "px"));

	robot
		.pairDevices({"frontLeftTank", "backLeftTank"}, "leftTank") // Pairing chassis left side
		.pairDevices({"frontRightTank", "backRightTank"}, "rightTank") // Pairing chassis right side
		.pairDevices({"leftFourbar", "rightFourbar"}, "fourbar"); // Pairing fourbar

	controller
		.linkAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y, []() {
			int leftVelocity = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / robot.chassisSensitivity;
			leftVelocity += (leftVelocity != 0) ? ((leftVelocity > 0) ? robot.wheelAddVelocity : -robot.wheelAddVelocity) : 0;

			robot.movePairMotors("leftTank", leftVelocity); 
		}) // Linking left side chassis to left joystick y axis
		.linkAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y, []() {
			int rightVelocity = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / robot.chassisSensitivity;
			rightVelocity += (rightVelocity != 0) ? ((rightVelocity > 0) ? robot.wheelAddVelocity : -robot.wheelAddVelocity) : 0;

			robot.movePairMotors("rightTank", rightVelocity); 
		}) // Linking right side chassis to right joystick y axis

		// Linking fourbar movement to left back buttons
		.linkDigital(pros::E_CONTROLLER_DIGITAL_L1, []() { robot.movePairMotors("fourbar", 80); }, []() { robot.movePairMotors("fourbar", 0); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_L2, []() { robot.movePairMotors("fourbar", -80); }, []() { robot.movePairMotors("fourbar", 0); })

		// Linking backlift movement to right back buttons
		.linkDigital(pros::E_CONTROLLER_DIGITAL_R1, []() { robot.getMotor("backLift").move_velocity(-80); }, []() { robot.getMotor("backLift").move_velocity(0); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_R2, []() { robot.getMotor("backLift").move_velocity(80); }, []() { robot.getMotor("backLift").move_velocity(0); })

		// Linking back drag movement to up and down arrows
		.linkDigital(pros::E_CONTROLLER_DIGITAL_UP, []() { robot.getMotor("backDrag").move_velocity(-80); }, []() { robot.getMotor("backDrag").move_velocity(0); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_DOWN, []() { robot.getMotor("backDrag").move_velocity(80); }, []() { robot.getMotor("backDrag").move_velocity(0); })

		.linkDigital(pros::E_CONTROLLER_DIGITAL_A, []() {
			robot.chassisSensitivity = (robot.chassisSensitivity > 3) ? 1 : robot.chassisSensitivity + 0.5;

			controller.setControllerText("Sensitivity < " + std::to_string(robot.chassisSensitivity));

			pros::delay(100);
		}, nullptr) // Linking controller chassis sensitivity to button a

		.linkDigital(pros::E_CONTROLLER_DIGITAL_B, []() { 
			controller.setControllerText("Resetting Fourbar");
    
			robot.getMotor("leftFourbar").move_relative(-robot.getMotor("leftFourbar").get_position(), 100);
			robot.getMotor("rightFourbar").move_relative(-robot.getMotor("rightFourbar").get_position(), 100);

			pros::delay(1000);

			controller.setControllerText("Fourbar Resetted");
		}, nullptr) // Linking fourbar reset to button b

		.linkDigital(pros::E_CONTROLLER_DIGITAL_X, []() {
			robot.wheelAddVelocity = (robot.wheelAddVelocity >= 100) ? 0 : robot.wheelAddVelocity + 50 ;

			controller.setControllerText("Wheel < " + std::to_string(robot.wheelAddVelocity));

			pros::delay(100);
		}, nullptr) // Linking extra wheel speed to button x

		.linkDigital(pros::E_CONTROLLER_DIGITAL_Y, []() {
			robot.getPiston("claw").toggle();

			controller.setControllerText("Front Claw < " + std::string(robot.getPiston("claw").get_value() ? "on" : "off"));

			pros::delay(200);
		}, nullptr); // Linking claw to button y

	robot
		.getVision("vision")
			.addSignature(YGOAL, KRONOS::Vision::signature_from_utility(YGOAL, 7473, 12171, 9822, -1985, -1565, -1776, 2.200, 0));
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
	std::cout << "Selecting auton..." << std::endl;

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
	std::cout << "Running auton..." << std::endl;

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