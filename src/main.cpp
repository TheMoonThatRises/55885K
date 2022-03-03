#include "KRONOS/kronos.h"
#include "KRONOS/helpers.h"

#define YGOAL 1
#define motorJoystickRatio 1.574

std::map<std::string, std::string> env {{"motorAcc", "0"}, {"maxAcc", "100"}, {"leftAcc", "0"}, {"rightAcc", "0"}};

KRONOS::Robot robot(env);
KRONOS::Controller controller(robot);
KRONOS::Autonomous auton(robot, controller, {"fl200_fr200_bl200_br200_lf200_rf200_ln700\nfl200_fr200_bl200_br200_lf-200_rf-200_ln800\ncl-200_ln200\nfl-100_fr-100_bl-100_br-100_ln2400\ncl200_ln200", "fl200_fr200_bl200_br200_lf200_rf200_kl200_ln700\nfl200_fr200_bl200_br200_lf-200_rf-200_kl200_ln800\ncl\nfl-200_fr-200_bl-200_br-200_kl200_lf20_rf20_ln1000\ntk_vs_0_1_px_50_110_leftTank_rightTank_80_-1\nkl-200_ln1000\nfl200_fr100_bl200_br100_ln2000", "tk_vs_0_1_px_50_110_leftTank_rightTank_30_-1", ""}, {"midGoals", "allGoals", "blueGoal", "None"}, 0);
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
		.addMotor(KRONOS::Device(KRONOS::Motor(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "frontRightTank", "fr"))
		.addMotor(KRONOS::Device(KRONOS::Motor(7, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "frontLeftTank", "fl"))
		.addMotor(KRONOS::Device(KRONOS::Motor(5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "backRightTank", "br"))
		.addMotor(KRONOS::Device(KRONOS::Motor(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_COAST), "backLeftTank", "bl"))

		// Adding fourbar motors
		.addMotor(KRONOS::Device(KRONOS::Motor(10, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "leftFourbar", "lf"))
		.addMotor(KRONOS::Device(KRONOS::Motor(9, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "rightFourbar", "rf"))

		// Adding back motors
		.addMotor(KRONOS::Device(KRONOS::Motor(12, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "backLift", "kl"))

		// Adding motor claw
		.addMotor(KRONOS::Device(KRONOS::Motor(19, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES, pros::E_MOTOR_BRAKE_HOLD), "claw", "cl"))

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
		.linkAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y, [&]() {
			if (std::stoi(robot.env.at("rightAcc")) < controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * motorJoystickRatio && controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) > 0)
				robot.env.at("rightAcc") = std::to_string(std::stoi(robot.env.at("rightAcc")) + 50);
			else if (std::stoi(robot.env.at("rightAcc")) > controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * motorJoystickRatio && controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) < 0)
				robot.env.at("rightAcc") = std::to_string(std::stoi(robot.env.at("rightAcc")) - 50);

			robot.movePairMotors("leftTank", std::stoi(robot.env.at("rightAcc")));
		}, [&]() {
			robot.env.at("rightAcc") = "0";
		}) // Linking left side chassis to left joystick y axis
		.linkAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y, [&]() {
			if (std::stoi(robot.env.at("leftAcc")) < controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * motorJoystickRatio && controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 0)
				robot.env.at("leftAcc") = std::to_string(std::stoi(robot.env.at("leftAcc")) + 50);
			else if (std::stoi(robot.env.at("leftAcc")) > controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * motorJoystickRatio && controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) < 0)
				robot.env.at("leftAcc") = std::to_string(std::stoi(robot.env.at("leftAcc")) - 50);

			robot.movePairMotors("rightTank", std::stoi(robot.env.at("leftAcc")));
		}, [&]() {
			robot.env.at("leftAcc") = "0";
		}) // Linking right side chassis to right joystick y axis

		// Linking fourbar movement to left back buttons
		.linkDigital(pros::E_CONTROLLER_DIGITAL_L1, [&]() {
				// robot.queue.removeQueue("fourbarReset");
				if (std::stoi(robot.env.at("motorAcc")) < std::stoi(robot.env.at("maxAcc")))
					robot.env.at("motorAcc") = std::to_string(std::stoi(robot.env.at("motorAcc")) + 20);
				robot.movePairMotors("fourbar", std::stoi(robot.env.at("motorAcc"))); 
			}, [&]() {
				// robot.queue.removeQueue("fourbarReset");
				if (std::stoi(robot.env.at("motorAcc")) > 0)
					robot.env.at("motorAcc") = std::to_string(std::stoi(robot.env.at("motorAcc")) - 5);
				robot.movePairMotors("fourbar", std::stoi(robot.env.at("motorAcc")));
			 })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_L2, [&]() {
				// robot.queue.removeQueue("fourbarReset");
				if (std::stoi(robot.env.at("motorAcc")) > -std::stoi(robot.env.at("maxAcc")))
					robot.env.at("motorAcc") = std::to_string(std::stoi(robot.env.at("motorAcc")) - 20);
				robot.movePairMotors("fourbar", std::stoi(robot.env.at("motorAcc")));
			}, [&]() {
				// robot.queue.removeQueue("fourbarReset");
				if (std::stoi(robot.env.at("motorAcc")) < 0)
					robot.env.at("motorAcc") = std::to_string(std::stoi(robot.env.at("motorAcc")) + 5);
				robot.movePairMotors("fourbar", std::stoi(robot.env.at("motorAcc")));
		})

		// Linking backlift movement to right back buttons
		.linkDigital(pros::E_CONTROLLER_DIGITAL_R1, []() { robot.getMotor("backLift").move_velocity(-80); }, []() { robot.getMotor("backLift").move_velocity(0); })
		.linkDigital(pros::E_CONTROLLER_DIGITAL_R2, []() { robot.getMotor("backLift").move_velocity(80); }, []() { robot.getMotor("backLift").move_velocity(0); })

		.linkDigital(pros::E_CONTROLLER_DIGITAL_B, []() {
			controller.setControllerText("Resetting Fourbar");
			robot.queue.addQueue(0, QueueItem("fourbarReset", [&](std::vector<std::string> env) {
				robot.getMotor("leftFourbar").move_absolute(0, 100);
				robot.getMotor("rightFourbar").move_absolute(0, 100);
				
				if (robot.getMotor("leftFourbar").get_position() == 0 && robot.getMotor("rightFourbar").get_position() == 0) {
					robot.queue.removeQueue("fourbarReset");
					controller.setControllerText("Fourbar Resetted");
				}
			}, {}));
		}, nullptr) // Linking fourbar reset to button b

		.linkDigital(pros::E_CONTROLLER_DIGITAL_A, []() {
			for (KRONOS::Motor motor : robot.getMotorPairs("rightTank"))
				motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

			for (KRONOS::Motor motor : robot.getMotorPairs("leftTank"))
				motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}, []() {
			for (KRONOS::Motor motor : robot.getMotorPairs("rightTank"))
				motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

			for (KRONOS::Motor motor : robot.getMotorPairs("leftTank"))
				motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		}) // Linking brake mode to button a

		.linkDigital(pros::E_CONTROLLER_DIGITAL_Y, []() {
			robot.queue.addQueue(0, QueueItem("claw", [&](std::vector<std::string> env) {
				if (env.at(0) == "down")
					robot.getMotor("claw").move_absolute(-150, 100);
				else
					robot.getMotor("claw").move_absolute(0, 100);

				if ((robot.getMotor("claw").get_position() >= -10 && robot.getMotor("claw").get_position() <= 30 && env.at(0) == "up") || (robot.getMotor("claw").get_position() <= -160 && robot.getMotor("claw").get_position() >= 140 && env.at(0) == "down"))
					robot.queue.removeQueue("claw");
			}, {(robot.getMotor("claw").get_position() > -100) ? "down" : "up"}));
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