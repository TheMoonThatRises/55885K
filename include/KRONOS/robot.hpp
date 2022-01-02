#include "helpers.h"
#include "motor.hpp"
#include "piston.hpp"
#include "controller.hpp"
#include "device.hpp"


#ifndef _KRONOS_ROBOT_HPP_
#define _KRONOS_ROBOT_HPP_

namespace KRONOS {
    class Robot {
        private:
            template<class T>
            T getType(std::vector<Device<T>> devices, const std::string& name);
            template<class T>
            Robot& addType(std::vector<Device<T>> &devices, Device<T> device);
            template<class C>
            Robot& linkController(std::map<C, std::string> &typeMap, C controls, std::string deviceName);
        protected:
            std::vector<Device<Motor>> motors;
            std::vector<Device<Piston>> pistons;
            std::map<std::string, pros::ADIDigitalIn> buttons;
            std::map<std::string, std::vector<std::string>> devicePairs;
            std::map<pros::controller_analog_e_t, std::string> controllerLinkAnalog;
            std::map<pros::controller_digital_e_t, std::string> controllerLinkDigital;
            Controller master;
        public:
            enum device_types {
                MOTOR,
                PISTON
            };

            Robot();
            Robot& addMotor(Device<Motor> motor);
            Robot& addPiston(Device<Piston> piston);
            Robot& addButton(const std::string& name, pros::ADIDigitalIn button);
            Robot& pairDevices(const std::vector<std::string>& callNames, const std::string& pairName);
            Robot& linkDeviceController(pros::controller_analog_e_t control, std::string deviceName);
            Robot& linkDeviceController(pros::controller_digital_e_t control, std::string deviceName);
            Motor getMotor(const std::string& name);
            Piston getPiston(const std::string& name);
            template<class T>
            std::vector<Device<T>> getPairs(const device_types type, const std::string& name);
            pros::ADIDigitalIn getButton(const std::string& name);
            Controller getController();
            void movePairMotors(const std::string& pairName, int32_t velocity);
            void movePairMotors(const std::string& pairName, int32_t velocity, double distance);
            void controllerListener();
    };
}

#endif