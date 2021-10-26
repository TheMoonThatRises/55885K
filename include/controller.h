#include "main.h"

class controller {
    public:
        static pros::Controller master;
        void moveChassis();
        void moveFourbar();
        void setChassisBrake();
        void setFourbarBrake();
};