#include "main.h"
#include <string>

class controller {
    public:
        static pros::Controller master;
        static void moveChassis();
        static void moveFourbar();
        static void moveRingSystem();
        static void changeChassisBrake();
        static void changeFourbarBrake();
        static void changeChassisSpeed();
        static void setControllerText(std::string text);
};