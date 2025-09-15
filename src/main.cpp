/*
List of compile macros

KRONOS_NOASSERTS
KRONOS_LOG_COUT
KRONOS_LOG_FILE
KRONOS_STRICT_DEVICE_GETTER
KRONOS_SAFETY_CHECKS
KRONOS_DEVICE_USE_MUTEX
KRONOS_STRICT_MUTEX
KRONOS_PRODUCTION
*/

/*
  Include main libraries
*/
#include "KRONOS/kronos.hpp" // Include KRONOS library
#include "main.h" // Include run header file

using kronos::base::Robot;
using kronos::assets::device_face;
using kronos::assets::controller_type;
using kronos::assets::KRONOS_MSDELAY;

using kronos::assets::Logger;

using kronos::base::Motor;
using kronos::base::Vision;
using kronos::base::Controller;
using kronos::base::LineTracker;

Robot robot;

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
    // Device initialisers
    .add_device(new Controller({}))

    // chassis devices
    .add_device("top_left", new Motor({.port=17, .face=device_face::K_NORTHWEST}))
    .add_device("top_right", new Motor({.port=18, .face=device_face::K_NORTHEAST}))

    .add_device("bottom_left", new Motor({.port=20, .face=device_face::K_SOUTHWEST}))
    .add_device("bottom_right", new Motor({.port=19, .face=device_face::K_SOUTHEAST}))

    // flywheel intake
    .add_device("flywheel_left", new Motor({.port=11, .gearset=pros::MotorGear::blue}))
    .add_device("flywheel_right", new Motor({.port=12, .gearset=pros::MotorGear::blue}))

    // sensors
    .add_device("vision", new Vision({.port=9}))
    // .add_device("imu", new KRONOS::Imu({.port=10}))
    // .add_device("gps", new KRONOS::GPS({.port=8}))
    // .add_device("line", new KRONOS::LineTracker({.port='C'}))

    // set chassis settings
    .set_chassis_motors(robot.get_multiple_devices({"top_left", "top_right", "bottom_left", "bottom_right"}))

    // chassis controls
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &velocity) {
      robot.move_chassis(velocity[0] * 3, velocity[1] * 3, velocity[2] / 1.8);
    })

    // flywheel controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &pressed) {
      robot.get_device<Motor>("flywheel_left")->move_velocity(pressed[0] ? 50. : pressed[1] ? -50. : 0.);
      robot.get_device<Motor>("flywheel_right")->move_velocity(pressed[0] ? -50. : pressed[1] ? 50. : 0.);
    });

  Vision* vision = robot.get_device<Vision>("vision");
  vision->add_signature(1, Vision::signature_from_utility(1, -1, 1, 0, -1, 1, 0, 3.000, 0));
  vision->set_exposure(40);

  Logger::info("Finish initializing Robot...");
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
  robot.kill_all_tasks();

  Vision* vision = robot.get_device<Vision>("vision");
  vision->set_signature(1);

  robot.sleep(1000);

  pros::vision_object_s_t line_object {};
  pros::vision_object_s_t tmp_object {};

  while (!robot.get_controller(controller_type::C_MASTER)->get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    double speed = 50;
    double angle = 0;

    int count = vision->get_object_count();

    for (int i = 0; i < count; ++i) {
      tmp_object = vision->get_by_sig(i, 1);

      if (
        tmp_object.top_coord - tmp_object.height < 20 &&
        abs(VISION_FOV_WIDTH / 2 - tmp_object.x_middle_coord) < 40
      ) {
        line_object = tmp_object;

        break;
      }
    }

    angle = std::min(std::max(-(VISION_FOV_WIDTH / 2 - line_object.x_middle_coord) / 2, -45), 45);

    robot.move_chassis(
      abs(speed * sin(angle)),
      speed * cos(angle),
      0);

    robot.sleep(KRONOS_MSDELAY);
  }

  robot.event_initialiser();
}
