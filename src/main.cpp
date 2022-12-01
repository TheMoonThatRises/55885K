/*
  Define environmental variables that control how KRONOS works
*/
// #define KRONOS_LOG_COUT
#define KRONOS_LOG_FILE
#define KRONOS_STRICT_DEVICE_ASSIGNMENT
#define KRONOS_STRICT_DEVICE_GETTER

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
  KLog::Log::info("Initializing Robot...");

  /*
    Add initialization code here
    Includes global variables, robot helper classes, etc
  */

  robot
    .set_side(KUtil::S_NONE)

    .add_device("topright", new KRONOS::Motor({.port=4, .reverse=true}))
    .add_device("topleft", new KRONOS::Motor({.port=5, .face=KRONOS::K_NORTHWEST}))
    .add_device("bottomright", new KRONOS::Motor({.port=3, .reverse=true, .face=KRONOS::K_SOUTHEAST}))
    .add_device("bottomleft", new KRONOS::Motor({.port=1}))

    .add_device("claw", new KRONOS::Motor({.port=14, .brakemode=pros::E_MOTOR_BRAKE_HOLD}))
    .add_device("clawrotate", new KRONOS::Motor({.port=15, .brakemode=pros::E_MOTOR_BRAKE_HOLD}))

    .add_device("button", new KRONOS::Button({.port='A'}))

    // .add_device("roller", new KRONOS::Motor({.port=14}))

    // .add_device("color", new KRONOS::Color({.port=18}))

    .add_device(new KRONOS::Controller({.id=pros::E_CONTROLLER_MASTER}))

    .set_chassis_motors(robot.get_multiple_devices({"topleft", "topright", "bottomleft", "bottomright"}))

    .add_controller_link([&]() {
      try {
        KLog::Log::info(dynamic_cast<KRONOS::Button*>(robot.get_device("asdf"))->get_value() ? "true" : "false");
      } catch (const std::exception &except) {
        KLog::Log::error(except.what());
      }
    })

    // Create chassis listener
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &analogs) {
      robot.move_chassis(analogs[0], analogs[1], analogs[2] / 1.8);
    })

    // Create claw listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &values) {
      dynamic_cast<KRONOS::Motor*>(robot.get_device("claw"))->move_velocity_pid(
        values.at(0) ? 100 :
          values.at(1) ? -100 : 0
      );
    })

    // Create claw rotation listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_L2}, [&](const std::vector<bool> &values) {
      dynamic_cast<KRONOS::Motor*>(robot.get_device("clawrotate"))->move_velocity_pid(
        values.at(0) ? 100 :
          values.at(1) ? -100 : 0
      );
    });

    // Create roller listener
    // .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &values) {
    //   bool bvalue = values.at(0) || values.at(1),
    //        cvalue = values.at(0);

    //   KRONOS::Motor* motor = dynamic_cast<KRONOS::Motor*>(robot.get_device("roller"));

    //   pros::c::optical_raw_s_t rgb = dynamic_cast<KRONOS::Color*>(robot.get_device("color"))->get_raw();

    //   if (rgb.red > 12 && rgb.blue > 6 && rgb.blue > 6 && robot.side() == KRONOS::RED) {
    //     motor->move_velocity(bvalue ? (cvalue ? 250 : -250) : 0);
    //   } else {

    //   }
    // });

  KLog::Log::info("Finish initializing Robot...");
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

  robot.select_auton();
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
  while (true) {
    robot.controller_listener();

    pros::delay(MSDELAY); // Always have a pros::delay in a while (true) loop
  }
}
