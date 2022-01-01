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
        protected:
            std::vector<Device<Motor>> motors;
            std::vector<Device<Piston>> pistons;
            std::map<std::string, pros::ADIDigitalIn> buttons;
            const Controller master;
        public:
            Robot(std::vector<Device<Motor>> motorObject, std::vector<Device<Piston>> pistonObject, std::map<std::string, pros::ADIDigitalIn> buttonMap);
            Motor getMotor(std::string name);
            Piston getPiston(std::string name);
            pros::ADIDigitalIn getButton(std::string name);
            Controller getController();
    };
}

#endif