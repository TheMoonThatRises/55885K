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
        protected:
            std::vector<Device<Motor>> motors;
            std::vector<Device<Piston>> pistons;
            std::map<std::string, pros::ADIDigitalIn> buttons;
            std::map<std::string, std::vector<std::string>> motorPairs;
            const Controller master;
        public:
            Robot();
            Robot& addMotor(Device<Motor> motor);
            Robot& addPiston(Device<Piston> piston);
            Robot& addButton(const std::string& name, pros::ADIDigitalIn button);
            Motor getMotor(const std::string& name);
            Piston getPiston(const std::string& name);
            pros::ADIDigitalIn getButton(const std::string& name);
            Controller getController();
            void pairMotors(const std::vector<std::string>& callNames, const std::string& pairName);
            void movePair(const std::string& pairName, int32_t velocity);
            void movePair(const std::string& pairName, int32_t velocity, double distance);
            void controllerListener();
    };
}

#endif