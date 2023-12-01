/*
  Define environmental variables that control how KRONOS works
*/
// #define KRONOS_SAFETY_CHECKS
#define KRONOS_STRICT_DEVICE_GETTER
// #define KRONOS_STRICT_MUTEX

/*
  Include main libraries
*/
#include "kronos.hpp" // Include KRONOS library
#include "main.h" // Include run header file

KRONOS::Robot robot;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  /*
    Add initialization code here
    Includes global variables, robot helper classes, etc
  */

  robot
    .global_set<int>("side", KUtil::S_RED)
    // Device initialisers
    .add_device(new KRONOS::Controller({}))

    // chassis devices
    .add_device("leftone", new KRONOS::Motor({.port=1, .reverse=true, .face=KRONOS::K_NORTHWEST}))
    .add_device("lefttwo", new KRONOS::Motor({.port=2, .reverse=true, .face=KRONOS::K_WEST}))
    .add_device("leftthree", new KRONOS::Motor({.port=3, .reverse=true, .face=KRONOS::K_SOUTHWEST}))

    .add_device("rightone", new KRONOS::Motor({.port=4, .reverse=true, .face=KRONOS::K_NORTHEAST}))
    .add_device("righttwo", new KRONOS::Motor({.port=5, .reverse=true, .face=KRONOS::K_EAST}))
    .add_device("rightthree", new KRONOS::Motor({.port=6, .reverse=true, .face=KRONOS::K_SOUTHEAST}))

    // launcher device
    .add_device("catapult", new KRONOS::Motor({.port=7, .gearset=pros::E_MOTOR_GEAR_RED,}))

    // intake device
    .add_device("intake", new KRONOS::Motor({.port=8, .gearset=pros::E_MOTOR_GEAR_BLUE}))

    // launcher distance trigger
    .add_device("ltrigger", new KRONOS::Button({.port='A'}))

    .set_chassis_motors(robot.get_multiple_devices({"leftone", "lefttwo", "leftthree", "rightone", "righttwo", "rightthree"}))
    .set_chassis_use_pid(true)
    .set_chassis_pid({.kP=0.0, .kI=0.0, .kD=0.0})

    // chassis controls
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &velocity) {
      robot.move_chassis(-velocity[0], velocity[1], velocity[2] / 1.8);
    })

    // launcher controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &pressed) {
      KRONOS::Motor *catapult = robot.get_device<KRONOS::Motor>("catapult");
      KRONOS::Button *ltrigger = robot.get_device<KRONOS::Button>("ltrigger");

      if (pressed[1]) {
        catapult->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
      } else if (pressed[0]) {
        catapult->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

        if (ltrigger->get_value()) {
          catapult->move_velocity(100, 300);
        }

        while (!ltrigger->get_value()) {
          catapult->move_velocity(50, 10);
        }
      }
    })

    .set_auton_assets(robot.get_controller(KRONOS::C_MASTER))

    .add_auton("offense", [&]() {

    })
    .add_auton("defensive", [&]() {

    })
    .add_auton("skills", [&]() {

    })

    // intake controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_L2}, [&](const std::vector<bool> &pressed) {
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(pressed[0] ? -600 : pressed[1] ? 600 : 0);
    });


  KLog::Log::info("Finish initializing Robot...");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  /*
    Load auton threads if it is autonomous and disabled
  */
  robot.kill_all_tasks();

  if (pros::competition::is_autonomous()) {
    robot.load_auton_threads();
  }
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  /*
    Initialize autonomous selector here
    Make sure to have a while (true) loop to select auton, and a way to break out of the loop
  */

  robot.load_auton_threads();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  /*
    Run autonomous code here
  */

  robot.run_auton();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  robot.event_initialiser();
}
