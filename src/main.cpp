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
    .global_set<std::function<void(KRONOS::Motor*)>>("full_launch_rotate", [&](KRONOS::Motor *catapult) {
      catapult->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

      catapult->move_velocity(-50);
    })

    // chassis devices
    .add_device("leftone", new KRONOS::Motor({.port=1, .reverse=true, .face=KRONOS::K_NORTHWEST}))
    .add_device("lefttwo", new KRONOS::Motor({.port=2, .reverse=true, .face=KRONOS::K_SOUTHWEST}))

    .add_device("rightone", new KRONOS::Motor({.port=3, .reverse=true, .face=KRONOS::K_NORTHEAST}))
    .add_device("righttwo", new KRONOS::Motor({.port=4, .reverse=true, .face=KRONOS::K_SOUTHEAST}))

    // launcher device
    .add_device("catapult", new KRONOS::Motor({.port=5, .gearset=pros::E_MOTOR_GEAR_RED, .brakemode=pros::E_MOTOR_BRAKE_HOLD}))

    // lift bar
    .add_device("leftlift", new KRONOS::Motor({.port=6, .reverse=true, .gearset=pros::E_MOTOR_GEAR_RED, .brakemode=pros::E_MOTOR_BRAKE_HOLD}))
    .add_device("rightlift", new KRONOS::Motor({.port=7, .gearset=pros::E_MOTOR_GEAR_RED, .brakemode=pros::E_MOTOR_BRAKE_HOLD}))

    // intake device
    // .add_device("intake", new KRONOS::Motor({.port=9, .gearset=pros::E_MOTOR_GEAR_GREEN}))

    // set chassis settings
    .set_chassis_motors(robot.get_multiple_devices({"leftone", "lefttwo", "rightone", "righttwo"}))
    .set_chassis_use_pid(false)
    .set_chassis_pid({.kP=0.0, .kI=0.0, .kD=10.0})

    // chassis controls
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &velocity) {
      robot.move_chassis(-velocity[0], velocity[1], velocity[2] / 1.8);
    })

    // launcher controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_UP, pros::E_CONTROLLER_DIGITAL_DOWN}, [&](const std::vector<bool> &pressed) {
      KRONOS::Motor *catapult = robot.get_device<KRONOS::Motor>("catapult");

      if (pressed[1]) {
        catapult->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
      } else if (pressed[0]) {
        robot.global_get<std::function<void(KRONOS::Motor*)>>("full_launch_rotate")->operator()(catapult);
      } else {
        catapult->move_velocity(0);
      }
    })

    // intake controls
    // .add_controller_link({pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_L2}, [&](const std::vector<bool> &pressed) {
    //   robot.get_device<KRONOS::Motor>("intake")->move_velocity(pressed[0] ? 600 : pressed[1] ? -600 : 0);
    // })

    // lift controls
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &pressed) {
      robot.get_device<KRONOS::Motor>("leftlift")->move_velocity(pressed[0] ? -50 : pressed[1] ? 50 : 0);
      robot.get_device<KRONOS::Motor>("rightlift")->move_velocity(pressed[0] ? -50 : pressed[1] ? 50 : 0);
    })

    // expansion controls
    // .add_controller_link(pros::E_CONTROLLER_DIGITAL_A, [&](const bool &pressed) {
    //   if (pressed) {
    //     robot.get_device<KRONOS::Piston>("expansion")->toggle();
    //     robot.sleep(500);
    //   }
    // })

    .set_auton_assets(robot.get_controller(KRONOS::C_MASTER))

    .add_auton("offense", [&]() {
      robot.move_chassis(50, 0, 0, 2000);

    })
    .add_auton("defensive", [&]() {
      robot.get_device<KRONOS::Motor>("catapult")->move_velocity(-50, 600);

      robot.get_device<KRONOS::Motor>("leftlift")->move_velocity(-50);
      robot.get_device<KRONOS::Motor>("rightlift")->move_velocity(-50);

      robot.sleep(1600);

      robot.get_device<KRONOS::Motor>("catapult")->move_velocity(-50, 1000);

      robot.get_device<KRONOS::Motor>("leftlift")->move_velocity(50);
      robot.get_device<KRONOS::Motor>("rightlift")->move_velocity(50);

      robot.sleep(1700);

      robot.get_device<KRONOS::Motor>("leftlift")->move_velocity(0);
      robot.get_device<KRONOS::Motor>("rightlift")->move_velocity(0);

      robot.move_chassis(10, 0, 13, 1000);
      robot.move_chassis(0, 0, 20, 2300);
      robot.move_chassis(-50, 0, -3, 2000);

      robot.move_chassis(50, 0, 3, 1000);
    })
    .add_auton("skills", [&]() {

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
