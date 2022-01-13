#include "KRONOS/kronos.h"
#include "KRONOS/helpers.h"

KRONOS::Robot robot(0, 1);
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

		.addMotor(KRONOS::Device(KRONOS::Motor(11, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backDrag", "bd"))
		.addMotor(KRONOS::Device(KRONOS::Motor(12, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backLift", "kl"))

		.addPiston(KRONOS::Device(KRONOS::Piston('a'), "claw", "cl"))

		.addButton("select", pros::ADIDigitalIn('b'))
		.addButton("lock", pros::ADIDigitalIn('c'));

	robot
		.pairDevices({"frontLeftTank", "backLeftTank"}, "leftTank")
		.pairDevices({"frontRightTank", "backRightTank"}, "rightTank")
		.pairDevices({"leftFourbar", "rightFourbar"}, "fourbar");

	controller
		.linkAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y, [](pros::controller_analog_e_t control) {
			int leftVelocity = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / robot.chassisSensitivity;
			leftVelocity += (leftVelocity != 0) ? ((leftVelocity > 0) ? robot.wheelAddVelocity : -robot.wheelAddVelocity) : 0;

			robot.movePairMotors("leftTank", leftVelocity); 
		})
		.linkAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y, [](pros::controller_analog_e_t control) {
			int rightVelocity = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / robot.chassisSensitivity;
			rightVelocity += (rightVelocity != 0) ? ((rightVelocity > 0) ? robot.wheelAddVelocity : -robot.wheelAddVelocity) : 0;

			robot.movePairMotors("rightTank", rightVelocity); 
		})

		.linkDigital(pros::E_CONTROLLER_DIGITAL_L1, [](pros::controller_digital_e_t control) { robot.movePairMotors("fourbar", 80); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_L2, [](pros::controller_digital_e_t control) { robot.movePairMotors("fourbar", -80); })

		.linkDigital(pros::E_CONTROLLER_DIGITAL_R1, [](pros::controller_digital_e_t control) { robot.getMotor("backLift").move_velocity(80); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_R2, [](pros::controller_digital_e_t control) { robot.getMotor("backLift").move_velocity(-80); })

		.linkDigital(pros::E_CONTROLLER_DIGITAL_UP, [](pros::controller_digital_e_t) { robot.getMotor("backDrag").move_velocity(80); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_DOWN, [](pros::controller_digital_e_t) { robot.getMotor("backDrag").move_velocity(-80); })

		.linkDigital(pros::E_CONTROLLER_DIGITAL_A, [](pros::controller_digital_e_t control) {
			robot.chassisSensitivity = (robot.chassisSensitivity > 3) ? 1 : robot.chassisSensitivity + 0.5;

			controller.setControllerText("Sensitivity < " + std::to_string(robot.chassisSensitivity));

			pros::delay(100);
		})

		.linkDigital(pros::E_CONTROLLER_DIGITAL_B, [](pros::controller_digital_e_t control) { 
			controller.setControllerText("Resetting Fourbar");
    
			robot.getMotor("leftFourbar").move_relative(-robot.getMotor("leftFourbar").get_position(), 100);
			robot.getMotor("rightFourbar").move_relative(-robot.getMotor("rightFourbar").get_position(), 100);

			pros::delay(1000);

			controller.setControllerText("Fourbar Resetted");
		})

		.linkDigital(pros::E_CONTROLLER_DIGITAL_X, [](pros::controller_digital_e_t) {
			robot.wheelAddVelocity = (robot.wheelAddVelocity >= 100) ? 0 : robot.wheelAddVelocity + 50 ;

			controller.setControllerText("Wheel < " + std::to_string(robot.wheelAddVelocity));

			pros::delay(100);
		})

		.linkDigital(pros::E_CONTROLLER_DIGITAL_Y, [](pros::controller_digital_e_t) {
			robot.getPiston("claw").set_value(!robot.getPiston("claw").get_value());

			controller.setControllerText("Front Claw < " + std::string(robot.getPiston("claw").get_value() ? "on" : "off"));

			pros::delay(200);
		});
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