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
        void resetFourbar();
        void changeChassisSpeed();
        void changeChassisSensitivity();
        void moveClaw();
        void moveBackLift();
        void setControllerText(const std::string& text);
};

#endif