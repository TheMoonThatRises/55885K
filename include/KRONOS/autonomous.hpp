#include "helpers.h"
#include "robot.hpp"

#ifndef _KRONOS_AUTONOMOUS_HPP_
#define _KRONOS_AUTONOMOUS_HPP_

namespace KRONOS {
    class Autonomous {
        private:
            Robot &robot;
            const std::vector<std::string> autons,
                                           autonToString;
            int auton;
        public:
            Autonomous(Robot &robot);

            void runAuton();
            void selectAuton();
    };
}

#endif