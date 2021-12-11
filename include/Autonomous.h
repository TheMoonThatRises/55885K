#include "main.h"
#include <string>


#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_

class Autonomous {
    private:
        const pros::ADIDigitalIn selectAutonB,
                                 lockAutonB;
    public:
        Autonomous();

        static void loadRunString(const std::string& auton);
        void selectAuton();
};

#endif