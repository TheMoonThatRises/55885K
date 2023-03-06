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
    .global_set("hasrumbled", false)
    .global_set("intakedir", 0)
    .global_set("plaunchtimes", 0)
    .global_set("aimmode", false)
    .global_set<std::function<void(std::string)>>("setsigtocolor", [&](const std::string &vision) {
      robot.get_controller(KRONOS::C_MASTER)->set_text("Setting color: " + std::string(*robot.global_get<int>("side") == KUtil::S_BLUE ? "Blue" : "Red"));
      robot.get_device<KRONOS::Vision>(vision)->set_signature(*robot.global_get<int>("side"));
    })
    .global_set<std::function<void(bool, int)>>("flywheel_func", [&](const bool &spin, const int &goSpeed) {
      auto* flywheelpid = robot.get_device<KRONOS::PIDDevice>("flywheel_pid");
      auto* flywheel1 = robot.get_device<KRONOS::Motor>("flywheel1");
      auto* flywheel2 = robot.get_device<KRONOS::Motor>("flywheel2");

      robot.global_set("flywheel", spin);

      if (robot.global_get<int>("plaunchtimes") == nullptr) {
        robot.global_set<int>("plaunchtimes", 1);
      }

      const double targetSpeed = (goSpeed + *robot.global_get<int>("plaunchtimes")) * 2;
      const double combinedSpeed = (flywheel1->get_actual_velocity() + flywheel2->get_actual_velocity());

      flywheelpid->set_max_speed(targetSpeed);

      const double speed = spin ? flywheelpid->pid(targetSpeed, combinedSpeed) * 10 : 0;

      // flywheelpid->add_consistency_value(combinedSpeed);

      // if (spin && !robot.get_device<KRONOS::Piston>("plauncher")->value() && flywheelpid->consistency(targetSpeed)) {
      //   robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(true);
      // }

      flywheel1->move_voltage(speed);
      flywheel2->move_voltage(speed);
    })
    .global_set<std::function<void(bool)>>("launcher_func", [&](const bool &launch) {
      if (launch) {
        robot.get_device<KRONOS::Piston>("plauncher")->toggle();
        robot.global_set("plaunchtimes", robot.global_get<int>("plaunchtimes") + 1);
        robot.sleep(500);
        robot.get_device<KRONOS::Piston>("plauncher")->toggle();
      }
    })

    // Device initialisers
    .add_device("topright", new KRONOS::Motor({.port=13, .reverse=true, .face=KRONOS::K_EAST}))
    .add_device("topleft", new KRONOS::Motor({.port=8, .reverse=true}))
    .add_device("bottomright", new KRONOS::Motor({.port=3, .reverse=true, .face=KRONOS::K_SOUTHEAST}))
    .add_device("bottomleft", new KRONOS::Motor({.port=1, .reverse=true, .face=KRONOS::K_SOUTH}))

    .add_device("aimcamera_pid", new KRONOS::PIDDevice(KExtender::P_ERROR, {.minspeed=-50, .maxspeed=50, .kP=0.2, .kI=0.0, .kD=0.0}, {}))
    .add_device("aimcamera", new KRONOS::Vision({.port=21}))

    .add_device("flywheel_pid", new KRONOS::PIDDevice(KExtender::P_NONE, {.minspeed=0, .kP=0.0, .kI=0.15, .kD=0.2}, {}))
    .add_device("flywheel1", new KRONOS::Motor({.port=14, .gearset=pros::E_MOTOR_GEARSET_06}))
    .add_device("flywheel2", new KRONOS::Motor({.port=16, .gearset=pros::E_MOTOR_GEARSET_06, .reverse=true}))

    .add_device("intake", new KRONOS::Motor({.port=15, .gearset=pros::E_MOTOR_GEARSET_18}))

    .add_device("roller", new KRONOS::Motor({.port=18, .gearset=pros::E_MOTOR_GEARSET_36}))

    .add_device("plauncher", new KRONOS::Piston({.port='H'}))

    .add_device("imu", new KRONOS::Imu({.port=5}))

    .add_device(new KRONOS::Controller({.id=pros::E_CONTROLLER_MASTER}))

    .set_chassis_motors(robot.get_multiple_devices({"topleft", "topright", "bottomleft", "bottomright"}))
    .set_auton_assets(robot.get_controller(KRONOS::C_MASTER))

    // Create chassis listener
    .add_controller_link({pros::E_CONTROLLER_ANALOG_LEFT_Y, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_RIGHT_X}, [&](const std::vector<double> &analogs) {
      robot.move_chassis(analogs[0], analogs[1], (*robot.global_get<bool>("aimmode") ? robot.get_device<KRONOS::PIDDevice>("aimcamera_pid")->pid(0, -robot.get_device<KRONOS::Vision>("aimcamera")->get_by_sig(0, *robot.global_get<int>("side")).x_middle_coord + 20) : (analogs[2] / 1.8)));
    })

    // Intake listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2}, [&](const std::vector<bool> &values) {
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(
        values[0] ? -200 :
         values[1] ? 200 : 0
      );
    })

    // Roller listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_X, pros::E_CONTROLLER_DIGITAL_B}, [&](const std::vector<bool> &values) {
      robot.get_device<KRONOS::Motor>("roller")->move_velocity(
        values[0] ? -160 :
          values[1] ? 160 : 0
      );
    })

    // Piston listener
    .add_controller_link(pros::E_CONTROLLER_DIGITAL_UP, [&](const bool &toggled) {
      robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(toggled);
    })

    // Flywheel listener
    .add_controller_link({pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_L2}, [&](const std::vector<bool> &spinSpeed) {
      robot.global_get<std::function<void(bool, int)>>("flywheel_func")->operator()(spinSpeed[0] || spinSpeed[1], spinSpeed[0] ? 350 : spinSpeed[1] ? 379 : 0);
    })

    // Aim mode
    .add_controller_link(pros::E_CONTROLLER_DIGITAL_A, [&](const bool &toggled) {
      robot.global_set("aimmode", toggled);
    })

    // Motor overheat listener
    .add_controller_link([&]() {
      #ifdef OVERHEAT_RUMBLE
        bool hasOverHeat = false;
        robot.global_set("hasrumbled", false);
        for (KRONOS::AbstractDevice *device : (robot.get_all() | std::ranges::views::filter([](const KRONOS::AbstractDevice *device) { return device->type() == KRONOS::K_MOTOR; }))) {
          if (KRONOS::to_motor(device)->is_over_temp()) {
            hasOverHeat = true;
            break;
          }
        }

        if (hasOverHeat && !robot.global_get<bool>("hasrumbled")) {
          robot.get_controller(KRONOS::C_MASTER)->rumble(".-.- .-.-");
        } else if (!hasOverHeat && robot.global_get<bool>("hasrumbled")) {
          robot.global_set("hasrumbled", true);
        }
      #endif
    })

    .add_auton("roller", [&]() {
      robot.move_chassis(20, 0, 0);
      robot.get_device<KRONOS::Motor>("roller")->move_velocity(20);
      robot.sleep(500);
      robot.move_chassis(0, 0, 0);
      robot.get_device<KRONOS::Motor>("roller")->move_velocity(0);
      robot.move_chassis(-20, 0, 0, 400);
      robot.move_chassis(0, 0, 50, 700);
      auto flywheelThread = new pros::Task([&](){
          while (true) {
            robot.global_get<std::function<void(bool, int)>>("flywheel_func")->operator()(true, 200);
            pros::delay(20);
          }
        }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "flywheelThread");
      robot.sleep(2500);
      robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(true);
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(-200);
      robot.sleep(3000);
      robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(true);
      robot.global_get<std::function<void(bool, int)>>("flywheel_func")->operator()(false, 0);
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(0);
      flywheelThread->remove();
    })
    .add_auton("rolleroffset", [&]() {
      auto flywheelThread = new pros::Task([&](){
          while (true) {
            robot.global_get<std::function<void(bool, int)>>("flywheel_func")->operator()(true, 210);
            pros::delay(20);
          }
        }, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "flywheelThread");
      robot.sleep(2500);
      robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(true);
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(-200);
      robot.sleep(3000);
      robot.global_get<std::function<void(bool)>>("launcher_func")->operator()(true);
      robot.global_get<std::function<void(bool, int)>>("flywheel_func")->operator()(false, 0);
      robot.get_device<KRONOS::Motor>("intake")->move_velocity(0);
      flywheelThread->remove();
      robot.move_chassis(50, 0, 0, 1200);
      robot.move_chassis(0, 0, 50, 770);
      robot.move_chassis(50, 0, 0);
      robot.get_device<KRONOS::Motor>("roller")->move_velocity(20);
      robot.sleep(900);
      robot.move_chassis(0, 0, 0);
      robot.get_device<KRONOS::Motor>("roller")->move_velocity(0);
    });

  robot.get_device<KRONOS::Vision>("aimcamera")
    ->add_signature(KUtil::S_RED, KRONOS::Vision::signature_from_utility(KUtil::S_RED, 8099, 8893, 8496, -1505, -949, -1227, 9.5, 0))
    .add_signature(KUtil::S_BLUE, KRONOS::Vision::signature_from_utility(KUtil::S_BLUE, -3441, -2785, -3113, 8975, 10355, 9665, 9.2, 0));

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
  if (pros::competition::is_autonomous()) {
    robot.load_auton_threads();
  } else {
    robot.event_deinitialize();
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
  robot.global_get<std::function<void(std::string)>>("setsigtocolor")->operator()("aimcamera");
  robot.event_initialiser();
}
