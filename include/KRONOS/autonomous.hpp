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
            const std::vector<std::string> autons,
                                           autonToString;
            int auton;
        public:
            /*
                @param robot
                @param controller
            */
            Autonomous(Robot& robot, Controller& controller);
            void runAuton();
            void selectAuton();
    };
}

#endif