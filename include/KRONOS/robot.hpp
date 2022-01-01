#include "main.h"
#include "motor.hpp"
#include "piston.hpp"
#include "controller.hpp"
#include "device.hpp"
#include <vector>
#include <string>
#include <map>


#ifndef _KRONOS_ROBOT_HPP_
#define _KRONOS_ROBOT_HPP_

namespace KRONOS {
    class Robot {
        private:
            template<class T>
            T getType(std::vector<Device<T>> devices, std::string name);
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
            Robot& addButton(std::string name, pros::ADIDigitalIn button);
            Motor getMotor(std::string name);
            Piston getPiston(std::string name);
            pros::ADIDigitalIn getButton(std::string name);
            Controller getController();
            void pairMotors(std::vector<std::string> callNames, std::string pairName);
            void movePair(std::string pairName, int32_t velocity);
            void movePair(std::string pairName, int32_t velocity, double distance);
    };
}

#endif