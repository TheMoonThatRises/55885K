// Ensure debug compile options turned off for production
#define KRONOS_PRODUCTION

/*
  Define environmental variables that control how KRONOS works
*/
// #define KRONOS_DEVICE_USE_MUTEX
#define KRONOS_LOG_COUT
#define KRONOS_LOG_FILE
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
    // Device initialisers
    .add_device(new KRONOS::Controller({}))

    // chassis devices
    .add_device("top_left", new KRONOS::Motor({.port=17, .face=KRONOS::K_NORTHWEST}))
    .add_device("top_right", new KRONOS::Motor({.port=18, .face=KRONOS::K_NORTHEAST}))

    .add_device("bottom_left", new KRONOS::Motor({.port=20, .face=KRONOS::K_SOUTHWEST}))
    .add_device("bottom_right", new KRONOS::Motor({.port=19, .face=KRONOS::K_SOUTHEAST}))

    // flywheel intake
    .add_device("flywheel_left", new KRONOS::Motor({.port=11, .gearset=pros::MotorGear::blue}))
    .add_device("flywheel_right", new KRONOS::Motor({.port=12, .gearset=pros::MotorGear::blue}))

    // sensors
    // .add_device("vision", new KRONOS::Vision({.port=9}))
    // .add_device("imu", new KRONOS::Imu({.port=10}))
    // .add_device("gps", new KRONOS::GPS({.port=8}))
    .add_device("line", new KRONOS::LineTracker({.port='C'}))

    // set chassis settings
    .set_chassis_motors(robot.get_multiple_devices({"top_left", "top_right", "bottom_left", "bottom_right"}))

    // chassis controls
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &velocity) {
      robot.move_chassis(velocity[0] * 3, velocity[1] * 3, velocity[2] / 1.8);
    })

    // flywheel controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &pressed) {
      robot.get_device<KRONOS::Motor>("flywheel_left")->move_velocity(pressed[0] ? 600. : pressed[1] ? -600. : 0.);
      robot.get_device<KRONOS::Motor>("flywheel_right")->move_velocity(pressed[0] ? -600. : pressed[1] ? 600. : 0.);
    })

    // auton settings
    .set_auton_assets(robot.get_controller(KRONOS::C_MASTER))

    // auton modes
    .add_auton("game", [&]() {
      KRONOS::LineTracker* tracker = robot.get_device<KRONOS::LineTracker>("line");

      double strafe = 50;

      while (true) {
        if (tracker->get_value() >= 2500) {
          strafe = -strafe;
        }

        robot.move_chassis(50, tracker->get_value() >= 2500 ? strafe : 0, 0);

        robot.sleep(20);
      }
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
  // robot.event_initialiser();
  robot.kill_all_tasks();

  KRONOS::Vision* vision = robot.get_device<KRONOS::Vision>("vision");
  KRONOS::LineTracker* tracker = robot.get_device<KRONOS::LineTracker>("line");
  tracker->calibrate();

  vision->add_signature(1, KRONOS::Vision::signature_from_utility(1, -1, 1, 0, -1, 1, 0, 3.000, 0));

  robot.sleep(1000);

  int prev_value = tracker->get_value();
  int curr_value = tracker->get_value();

  double speed = 50;
  double strafe_fwd = speed * sin(45);
  double strafe = -speed * cos(45);
  double last_time = 0;
  double curr_time = pros::millis();

  while (true) {
    curr_value = tracker->get_value();
    curr_time = pros::millis();

    if (curr_value >= 2500 && (curr_value - prev_value > 50 || curr_value >= 2700) && curr_time - last_time > 100) {
      strafe = -strafe;
      last_time = curr_time;
    }

    prev_value = curr_value;

    robot.move_chassis(
      curr_value >= 2500 ? strafe_fwd : speed,
      curr_value >= 2500 ? strafe : 0,
      0);

    robot.sleep(KUtil::KRONOS_MSDELAY);
  }
}
