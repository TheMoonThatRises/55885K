#include "main.h"
#include <string>


#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class Controller {
    public:
        Controller();

        pros::Controller master;
        void moveChassis();
        void moveFourbar();
        void resetFourbar(pros::Motor fourbarL, pros::Motor fourbarR);
        void changeChassisSpeed();
        void changeChassisMode();
        void changeChassisSensitivity();
        void setControllerText(std::string text);
};

#endif