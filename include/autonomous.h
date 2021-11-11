#include "main.h"
#include <string>

class autonomous {
    public:
        static pros::ADIDigitalIn selectAutonButton;
        static void loadRunFile(std::string auton);
        static void selectAuton();
};