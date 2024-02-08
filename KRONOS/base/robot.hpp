/*
  Copyright 2024 Peter Duanmu

  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef KRONOS_BASE_ROBOT_HPP_
#define KRONOS_BASE_ROBOT_HPP_

#include <functional>
#include <string>
#include <vector>
#include <utility>

#include "assets/statics.hpp"

#include "base/managers/autonmanager.hpp"
#include "base/managers/chassismanager.hpp"
#include "base/managers/controllermanager.hpp"
#include "base/managers/devicemanager.hpp"
#include "base/managers/safetymanager.hpp"
#include "base/managers/taskmanager.hpp"
#include "base/managers/varmanager.hpp"

#include "user/caster.hpp"
#include "user/memoryprofiler.hpp"

#include "internal/hotp.hpp"

#include "pros/rtos.hpp"

namespace KRONOS {
class Robot
: protected AutonomousManager,
  public DeviceManager,
  public ChassisManager,
  public ControllerManager,
  protected SafetyManager,
  public TaskManager,
  public VarManager {
 private:
  inline static const std::string _secret_key =
    KUUID::UUIDGenerator().generate_uuid();
  KOTP::HOTP _htop;

 protected:
    KMemoryProfiler::MemoryProfiler _memory_profiler;

 public:
    inline explicit Robot(
      bool use_memory_profiler = false,
      bool detailed_memory_profiler = true)
    : AutonomousManager(this, this, &_htop),
      ControllerManager(this),
      SafetyManager(this, this, this),
      VarManager(_secret_key),
      _htop(_secret_key),
      _memory_profiler(this, detailed_memory_profiler) {
      if (use_memory_profiler) {
        _memory_profiler.enable_memory_profiler();
      }

      VarManager::global_set<KUtil::side_color>(
        "side",
        KUtil::S_RED,
        _htop.get_code());

      KLog::Log::info("Constructing robot");
      KLog::Log::info(
        "Current status: Autonomous: " +
        std::to_string(pros::competition::is_autonomous()) +
        " Connected: " + std::to_string(pros::competition::is_connected()) +
        " Disabled: " + std::to_string(pros::competition::is_disabled()));

      (void) std::set_terminate([]() {
        try {
          std::rethrow_exception(std::current_exception());
        } catch (const AbstractKronosError &kronosexcept) {
          KLog::Log::error(
            "Uncaught KRONOS exception:" + std::string(kronosexcept.what()));
        } catch (const std::exception &exception) {
          KLog::Log::error(
            "Uncaught fatal exception: " + std::string(exception.what()));
        }

        std::abort();
      });
    }

    inline ~Robot() {
      KLog::Log::info("Destructuring robot class");
    }

    /*
      Adds a device to the robot

      @param name Name to pair with the device
      @param device Device to set

      @return Reference to initial robot class
    */
    inline Robot& add_device(const std::string &name, AbstractDevice *device) {
      DeviceManager::set(name, device);

      return *this;
    }

    /*
      Adds a controller to the robot

      @param controller Controller to set

      @return Reference to initial robot class
    */
    inline Robot& add_device(Controller *controller) {
      ControllerManager::add(controller);

      return *this;
    }

    /*
      Set chassis motors

      @param motors Vector of motor pointer

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_motors(const std::vector<Motor*> &motors) {
      ChassisManager::set_motors(motors);

      return *this;
    }

    /*
      Set chassis motors

      @param motors Vector of abstract device pointer

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_motors(
      const std::vector<AbstractDevice*> &devices) {
      std::vector<Motor*> motors(devices.size());

      (void) std::transform(
        devices.begin(),
        devices.end(),
        motors.begin(),
        [&](AbstractDevice *device) {
          return to_motor(device);
        });

      ChassisManager::set_motors(motors);

      return *this;
    }

    /*
      Set chassis odoms

      @param odoms

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_odoms(const std::vector<Rotation*> &odoms) {
      ChassisManager::set_odoms(odoms);

      return *this;
    }

    /*
      Set chassis odoms

      @param odoms

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_odoms(
      const std::vector<AbstractDevice*> &devices) {
      std::vector<Rotation*> motors(devices.size());

      (void) std::transform(
        devices.begin(),
        devices.end(),
        motors.begin(),
        [&](AbstractDevice *device) {
          return to_rotation(device);
        });

      ChassisManager::set_odoms(motors);

      return *this;
    }

    /*
      Sets pid gains for chassis

      @param gains The gains to update

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_pid(const KExtender::pid_consts &gains) {
      ChassisManager::set_pid_consts(gains);

      return *this;
    }

    /*
      Sets whether to use pid for chassis

      @param use Whether to use pid or not

      @return Reference to initial robot class
    */
    inline Robot& set_chassis_use_pid(const bool &use) {
      ChassisManager::use_pid(use);

      return *this;
    }

    /*
      Sets the devices that autonomous uses

      @param controller Main controller
    */
    inline Robot& set_auton_assets(KRONOS::Controller* controller) {
      AutonomousManager::set_assets(controller);

      return *this;
    }

    /*
      Sets analog link for listener to listen

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    inline Robot& add_controller_link(
      const pros::controller_analog_e_t &method,
      const single_analog_function& function,
      const controller_type &controller = C_MASTER) {
      ControllerManager::add(method, function, controller);

      return *this;
    }

    /*
      Sets analog links for listener to listen to

      @param methods Multiple controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    inline Robot& add_controller_link(
      const std::vector<pros::controller_analog_e_t> &methods,
      const multi_analog_function& function,
      const controller_type &controller = C_MASTER) {
      ControllerManager::add(methods, function, controller);

      return *this;
    }

    /*
      Sets digital link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    inline Robot& add_controller_link(
      const pros::controller_digital_e_t &method,
      const single_digital_function& function,
      const controller_type &controller = C_MASTER) {
      ControllerManager::add(method, function, controller);

      return *this;
    }

    /*
      Sets digital links for listener to listen to

      @param method Controller analog inputs
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    inline Robot& add_controller_link(
      const std::vector<pros::controller_digital_e_t> &method,
      const multi_digital_function& function,
      const controller_type &controller = C_MASTER) {
      ControllerManager::add(method, function, controller);

      return *this;
    }

    /*
      Runs a function every loop of the listener

      @param function Function to run

      @return Reference to initial robot class
    */
    inline Robot& add_controller_link(const std::function<void()>& function) {
      ControllerManager::add(function);

      return *this;
    }

    /*
      Saves auton to auton map

      @param name Name of auton
      @param auton Auton vector

      @return Reference to initial robot class
    */
    inline Robot& add_auton(
      const std::string &name,
      std::function<void()> auton) {
      AutonomousManager::add(name, auton);

      return *this;
    }

    /*
      Kills all other threads and runs auton threads
    */
    inline Robot& load_auton_threads() {
      (void) kill_all_tasks();
      AutonomousManager::load_auton();

      return *this;
    }

    /*
      Runs selected auton
    */
    inline Robot& run_auton() {
      (void) kill_all_tasks();
      AutonomousManager::run();

      return *this;
    }

    /*
      Listens to controller events

      @return Reference to initial robot class
    */
    inline Robot& event_initialiser() {
      (void) kill_all_tasks();
      ControllerManager::initialise_all();

      return *this;
    }

    inline Robot& kill_all_tasks() {
      AutonomousManager::unload_auton_threads();
      ControllerManager::event_deinitialize();

      return *this;
    }

    /*
      Manipulate groups of devices at the same time with the same command

      @param dnames Vector of names for devices
      @param func Function call that controls the device
      @param delay Delay after the manipDevices has ran
    */
    inline void manipulate_devices(
      const std::vector<std::string> &dnames,
      const std::function<void(std::pair<std::string, AbstractDevice*>)>& func,
      int delay = 50) {
      std::vector<std::pair<std::string, KRONOS::AbstractDevice *>> devices =
        DeviceManager::devices_by_key(dnames);

      for (const std::pair<std::string, AbstractDevice*> &device : devices) {
        func(device);
      }

      pros::delay(delay);
    }

    template<class T>
    inline Robot& global_set(const std::string &key, const T& value) {
      VarManager::global_set(key, value);

      return *this;
    }

    /*
      Delay's the robot's functionalities (Wrapper of pros::delay)

      @param milliseconds Length of sleep
    */
    inline static void sleep(const size_t &milliseconds) {
      pros::delay(milliseconds);
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_ROBOT_HPP_
