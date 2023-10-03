/*
  Define environmental variables that control how KRONOS works
*/
// #define KRONOS_STRICT_DEVICE_ASSIGNMENT //TODO: Fix
#define KRONOS_STRICT_DEVICE_GETTER
#define KRONOS_PRACTICE_DRIVE
// #define OVERHEAT_RUMBLE

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

    .add_device("leftone", new KRONOS::Motor({.port=17}))
    .add_device("lefttwo", new KRONOS::Motor({.port=18}))
    .add_device("leftthree", new KRONOS::Motor({.port=19}))

    .add_controller_link(pros::E_CONTROLLER_ANALOG_LEFT_Y, [&](const double &velocity) {
      robot.get_device<KRONOS::Motor>("leftone")->move_velocity(velocity);
      robot.get_device<KRONOS::Motor>("lefttwo")->move_velocity(velocity);
      robot.get_device<KRONOS::Motor>("leftthree")->move_velocity(velocity);
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
