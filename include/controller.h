#include "main.h"
#include <string>


#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class controller {
    public:
        static pros::Controller master;
        static void moveChassis();
        static void moveFourbar();
        static void moveIntake(); 
        static void resetFourbar();
        static void changeChassisSpeed();
        static void changeChassisMode();
        static void changeChassisSensitivity();
        static void setControllerText(std::string text);
};

#endif