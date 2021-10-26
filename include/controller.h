#include "main.h"

class controller {
    public:
        static pros::Controller master;
        static void moveChassis();
        static void moveFourbar();
        static void changeChassisBrake();
        static void changeFourbarBrake();
};