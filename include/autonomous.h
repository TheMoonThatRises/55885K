#include "main.h"
#include <string>


#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_

class autonomous {
    public:
        static pros::ADIDigitalIn selectAutonButton, lockinAutonButton;
        static void loadRunFile(const std::string& auton);
        static void selectAuton();
};

#endif