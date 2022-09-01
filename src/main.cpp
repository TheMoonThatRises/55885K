#include <iostream>

#include "kronos.hpp"
#include "main.h"

KRONOS::Robot robot;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	std::cout << "Initializing Robot..." << std::endl;

  /*
    Add initialization code here
    Includes global variables, robot helper classes, etc
  */

  robot
    .addDevice("topleft", new KRONOS::Motor({.port=1}))
    .addDevice("topright", new KRONOS::Motor({.port=5}))
    .addDevice("bottomleft", new KRONOS::Motor({.port=3, .reverse=true}))
    .addDevice("bottomright", new KRONOS::Motor({.port=4, .reverse=true}))
    
    .addDevice(new KRONOS::Controller({.id=pros::E_CONTROLLER_MASTER}))

    .addControllerLink(pros::E_CONTROLLER_ANALOG_LEFT_Y, 
      [&](const int &analogyleft) {
        robot.manipDevices({"topleft", "topright", "bottomleft", "bottomright"},
          [&](std::pair<std::string, KRONOS::AbstractDevice*> motor) {
	          dynamic_cast<KRONOS::Motor*>(motor.second)->safe_move_velocity(analogyleft);
          }
        );
      }
		)
    .addControllerLink(pros::E_CONTROLLER_ANALOG_RIGHT_X, 
      [&](const int &analogxright) {
        robot.manipDevices({"topleft", "topright", "bottomleft", "bottomright"},
          [&](std::pair<std::string, KRONOS::AbstractDevice*> motor) {
	          KRONOS::Motor* pmotor = dynamic_cast<KRONOS::Motor*>(motor.second);
	          pmotor->safe_move_velocity(pmotor->is_reversed() ? -analogxright : analogxright);
	        }
        );
      }
    );
  
  std::cout << "Finish initializing Robot..." << std::endl;
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

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
    Initilize autonomous selector here
    Make sure to have a while (true) loop to select auton, and a way to break out of the loop
  */
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
	while (true) {
    robot.controllerListener();

		pros::delay(MSDELAY); // Always have a pros::delay in a while (true) loop
	}
}
