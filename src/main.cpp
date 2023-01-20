/*
  Define environmental variables that control how KRONOS works
*/
#define KRONOS_LOG_COUT
#define KRONOS_LOG_FILE
// #define KRONOS_STRICT_DEVICE_ASSIGNMENT //TODO: Fix
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
  /*
    Add initialization code here
    Includes global variables, robot helper classes, etc
  */

  robot
    .global_set("flywheel", false)
    .global_set<std::function<void(bool)>>("flywheel_func", [&](const bool &spin) -> void {
      KRONOS::PIDDevice* flywheelpid = KRONOS::to_pid(robot.get_device("flywheel_pid"));
      KRONOS::Motor* flywheel1 = KRONOS::to_motor(robot.get_device("flywheel1"));
      KRONOS::Motor* flywheel2 = KRONOS::to_motor(robot.get_device("flywheel2"));

      robot.global_set("flywheel", spin);

      if (spin) {
        flywheelpid->set_exit_condition(KExtender::P_NONE);
      } else {
        flywheelpid->set_exit_condition(KExtender::P_ERROR);
      }

      flywheel1->move_velocity(flywheel2->move_velocity(flywheelpid->pid(200 * 2, flywheel1->get_actual_velocity() + flywheel2->get_actual_velocity())));
    })

    .set_side(KUtil::S_NONE)

    .add_device("topright", new KRONOS::Motor({.port=4, .reverse=true}))
    .add_device("topleft", new KRONOS::Motor({.port=12, .face=KRONOS::K_NORTHWEST}))
    .add_device("bottomright", new KRONOS::Motor({.port=3, .reverse=true, .face=KRONOS::K_SOUTHEAST}))
    .add_device("bottomleft", new KRONOS::Motor({.port=1}))

    .add_device("flywheel_pid", new KRONOS::PIDDevice(KExtender::P_NONE, {.kP=0.0, .kI=0.1, .kD=0.5}))
    .add_device("flywheel1", new KRONOS::Motor({.port=14, .gearset=pros::E_MOTOR_GEARSET_06}))
    .add_device("flywheel2", new KRONOS::Motor({.port=16, .gearset=pros::E_MOTOR_GEARSET_06}))

    .add_device("intake", new KRONOS::Motor({.port=18, .gearset=pros::E_MOTOR_GEARSET_18}))

    .add_device(new KRONOS::Controller({.id=pros::E_CONTROLLER_MASTER}))

    .set_chassis_motors(robot.get_multiple_devices({"topleft", "topright", "bottomleft", "bottomright"}))
    .set_auton_assets(nullptr, nullptr, robot.get_controller(KRONOS::C_MASTER))

    // Create chassis listener
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &analogs) {
      robot.move_chassis(analogs[0], analogs[1], analogs[2] / 1.8);
    })

    // Intake listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &values) {
      KRONOS::to_motor(robot.get_device("intake"))->move_velocity(
        values[0] ? -200 :
          values[1] ? 200 : 0
      );
    })

    // Flywheel listener
    .add_controller_link(pros::E_CONTROLLER_DIGITAL_L1, [&](const bool &spin) {
      robot.global_get<std::function<void(bool)>>("flywheel_func")->operator()(spin ? !*robot.global_get<bool>("flywheel") : *robot.global_get<bool>("flywheel"));

      if (spin) {
        robot.sleep(10);
      }
    });

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

  robot.global_get<std::function<void(bool)>>("flywheel_func")->operator()(true);

  robot.sleep(5000);

  KRONOS::to_motor(robot.get_device("intake"))->move_velocity(200, 5000);

  robot.global_get<std::function<void(bool)>>("flywheel_func")->operator()(false);

  // robot.run_auton();
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

    robot.sleep(KUtil::KRONOS_MSDELAY); // Always have a sleep in a while (true) loop
  }
}
