/*
  Copyright 2025 Peter Duanmu

  @file base/robot.hpp

  Stores robot controls and functions
*/

#ifndef KRONOS_BASE_ROBOT_HPP_
#define KRONOS_BASE_ROBOT_HPP_

#include <string>
#include <vector>

#include "KRONOS/assets/statics.hpp"
#include "KRONOS/assets/uuid.hpp"

#include "KRONOS/base/managers/autonmanager.hpp"
#include "KRONOS/base/managers/chassismanager.hpp"
#include "KRONOS/base/managers/controllermanager.hpp"
#include "KRONOS/base/managers/devicemanager.hpp"
#include "KRONOS/base/managers/safetymanager.hpp"
#include "KRONOS/base/managers/taskmanager.hpp"
#include "KRONOS/base/managers/varmanager.hpp"

#include "KRONOS/base/user/caster.hpp"
#include "KRONOS/base/user/memoryprofiler.hpp"

#include "KRONOS/internal/hotp.hpp"

#include "pros/rtos.hpp"

namespace kronos::base {
class Robot
: protected managers::AutonomousManager,
  public managers::DeviceManager,
  public managers::ChassisManager,
  public managers::ControllerManager,
  protected managers::SafetyManager,
  public managers::TaskManager,
  public managers::VarManager {
 private:
  inline static const std::string _secret_key =
    assets::UUIDGenerator().generate_uuid();
  internal::HOTP _htop;

 protected:
    user::MemoryProfiler _memory_profiler;

 public:
    explicit Robot(
      bool use_memory_profiler = false,
      bool detailed_memory_profiler = true);

    ~Robot();

    /*
      Adds a device to the robot

      @param name Name to pair with the device
      @param device Device to set

      @return Reference to initial robot class
    */
    Robot& add_device(const std::string& name, AbstractDevice *device);

    /*
      Adds a controller to the robot

      @param controller Controller to set

      @return Reference to initial robot class
    */
    Robot& add_device(Controller *controller);

    /*
      Set chassis motors

      @param motors Vector of motor pointer

      @return Reference to initial robot class
    */
    Robot& set_chassis_motors(const std::vector<Motor*>& motors);

    /*
      Set chassis motors

      @param motors Vector of abstract device pointer

      @return Reference to initial robot class
    */
    Robot& set_chassis_motors(const std::vector<AbstractDevice*>& devices);

    /*
      Set chassis odoms

      @param odoms

      @return Reference to initial robot class
    */
    Robot& set_chassis_odoms(const std::vector<Rotation*>& odoms);

    /*
      Set chassis odoms

      @param odoms

      @return Reference to initial robot class
    */
    Robot& set_chassis_odoms(const std::vector<AbstractDevice*>& devices);

    /*
      Sets pid gains for chassis

      @param gains The gains to update

      @return Reference to initial robot class
    */
    Robot& set_chassis_pid(const assets::pid_consts& gains);

    /*
      Sets whether to use pid for chassis

      @param use Whether to use pid or not

      @return Reference to initial robot class
    */
    Robot& set_chassis_use_pid(const bool& use);

    /*
      Sets the devices that autonomous uses

      @param controller Main controller
    */
    Robot& set_auton_assets(Controller* controller);

    /*
      Sets analog link for listener to listen

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    Robot& add_controller_link(
      const pros::controller_analog_e_t& method,
      const managers::single_analog_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Sets analog links for listener to listen to

      @param methods Multiple controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    Robot& add_controller_link(
      const std::vector<pros::controller_analog_e_t>& methods,
      const managers::multi_analog_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Sets digital link for listener to listen to

      @param method Controller analog input
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    Robot& add_controller_link(
      const pros::controller_digital_e_t& method,
      const managers::single_digital_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Sets digital links for listener to listen to

      @param method Controller analog inputs
      @param function Function to run
      @param controller Which controller input to read

      @return Reference to initial robot class
    */
    Robot& add_controller_link(
      const std::vector<pros::controller_digital_e_t>& method,
      const managers::multi_digital_function& function,
      const assets::controller_type& controller =
        assets::controller_type::C_MASTER);

    /*
      Runs a function every loop of the listener

      @param function Function to run

      @return Reference to initial robot class
    */
    Robot& add_controller_link(const std::function<void()>& function);

    /*
      Saves auton to auton map

      @param name Name of auton
      @param auton Auton vector

      @return Reference to initial robot class
    */
    Robot& add_auton(
      const std::string& name,
      std::function<void()> auton);

    /*
      Kills all other threads and runs auton threads
    */
    Robot& load_auton_threads();

    /*
      Runs selected auton
    */
    Robot& run_auton();

    /*
      Select default auton
    */
    Robot& set_current_auton(const std::string& name);

    /*
      Listens to controller events

      @return Reference to initial robot class
    */
    Robot& event_initialiser();

    Robot& kill_all_tasks();

    /*
      Manipulate groups of devices at the same time with the same command

      @param dnames Vector of names for devices
      @param func Function call that controls the device
      @param delay Delay after the manipDevices has ran
    */
    void manipulate_devices(
      const std::vector<std::string>& dnames,
      const std::function<void(std::pair<std::string, AbstractDevice*>)>& func,
      int delay = 50);

    template<class T>
    Robot& global_set(const std::string& key, const T& value);

    /*
      Delay's the robot's functionalities (Wrapper of pros::delay)

      @param milliseconds Length of sleep
    */
    static void sleep(const size_t& milliseconds);
};
}  // namespace kronos::base

#endif  // KRONOS_BASE_ROBOT_HPP_
