#include "main.h"
#include <string>

class controller {
    public:
        static pros::Controller master;
        static void moveChassis();
        static void moveFourbar();
        static void moveIntake(); 
        static void changeChassisBrake();
        static void changeChassisSpeed();
        static void changeChassisMode();
        static void changeChassisSensitivity();
        static void setControllerText(std::string text);
};