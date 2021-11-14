#include "main.h"
#include <string>


#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_

class Autonomous {
    private:
        const pros::ADIDigitalIn selectAutonButton, lockinAutonButton;
    public:
        Autonomous();

        static void loadRunFile(const std::string& auton);
        void selectAuton();
};

#endif