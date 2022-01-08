#include "helpers.h"
#include "motor.hpp"
#include "piston.hpp"
#include "device.hpp"


#ifndef _KRONOS_ROBOT_HPP_
#define _KRONOS_ROBOT_HPP_

namespace KRONOS {
    class Robot {
        private:
            template<class T>
            T getType(const std::vector<Device<T>>& devices, const std::string& name);
            template<class T>
            Robot& addType(std::vector<Device<T>>& devices, const Device<T>& device);
        protected:
            std::vector<Device<Motor>> motors;
            std::vector<Device<Piston>> pistons;
            std::map<std::string, pros::ADIDigitalIn> buttons;
            std::map<std::string, std::vector<std::string>> devicePairs;
            std::map<std::string, device_types> deviceType;
        public:
            Robot& addMotor(const Device<Motor>& motor);
            Robot& addPiston(const Device<Piston>& piston);
            Robot& addButton(const std::string& name, const pros::ADIDigitalIn& button);
            Robot& pairDevices(const std::vector<std::string>& callNames, const std::string& pairName);
            Motor getMotor(const std::string& name);
            Piston getPiston(const std::string& name);
            std::vector<std::string> getMotorPair(const std::string& callsign);
            device_types getDeviceType(const std::string& callsign);
            template<class T>
            std::vector<Device<T>> getPairs(const device_types& type, const std::string& name);
            pros::ADIDigitalIn getButton(const std::string& name);
            void movePairMotors(const std::string& pairName, const int32_t& velocity);
            void movePairMotors(const std::string& pairName, const int32_t& velocity, const double& distance);
            void activatePairPiston(const std::string& pairName, bool activated);
            void controllerListener();
    };
}

#endif