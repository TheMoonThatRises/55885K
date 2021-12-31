#include "main.h"
#include "robot.hpp"
#include <string>
#include <vector>


#ifndef _KRONOS_AUTONOMOUS_HPP_
#define _KRONOS_AUTONOMOUS_HPP_

namespace KRONOS {
    class Autonomous {
        private:
            const pros::ADIDigitalIn selectAutonB,
                                     lockAutonB;
            const Robot robot;
            const std::vector<std::string> autons,
                                           autonToString;
            int auton;
        public:
            Autonomous(Robot robot);

            void loadRunString();
            void selectAuton();
    };
}

#endif