#include "helpers.h"
#include "robot.hpp"
#include "controller.hpp"

#ifndef _KRONOS_AUTONOMOUS_HPP_
#define _KRONOS_AUTONOMOUS_HPP_

namespace KRONOS {
    class Autonomous {
        private:
            Robot& robot;
            Controller& controller;
            std::vector<std::string> autons,
                                     autonToString;
            int auton;
        public:
            /*
                @param robot
                @param controller
                @param autons All the autonomous strings.
                @param autonToString What the name of the auton is in the same index as in the vector `autons`.
            */
            Autonomous(Robot& robot, Controller& controller, const std::vector<std::string>& autons, const std::vector<std::string>& autonToString);
            void runAuton();
            void selectAuton();
    };
}

#endif