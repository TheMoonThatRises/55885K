#include "autonomous.h"
#include "robot.h"
#include "util.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


/**
 * The commands are:
 *      ls = Left chassis side
 *      rs = Right chassis side
 *      cT = Chassis move length - Required if ls or rs is used
 *      fb = Fourbar
 *      rm = Ring motor
 **/
void autonomous::loadRunFile(std::string auton) {
    std::string commands;
    std::istringstream Commands(auton);

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = util::splitString(commands, "_");
        int lsM = 0, rsM = 0, lrT = 0, fbM = 0, rmM = 0;

        for (std::string command : commandAr) {
            int dist = std::stoi(command.substr(2));
            std::string commandSt = command.substr(0, 2);
            std::cout << dist << " : " << commandSt << std::endl;
            
            if (commandSt == "ls") {
                lsM = dist;
            } else if (commandSt == "rs") {
                rsM = dist;
            } else if (commandSt == "cT") {
                lrT = dist;
            } else if (commandSt == "fb") {
                fbM = dist;
            } else if (commandSt == "rm") {
                rmM = dist;
            }
        }


        // Moving chassis

        robot::moveChassis(lsM, lsM, 0);

        pros::delay(lrT);

        robot::moveChassis(0, 0, 0);


        // Moving fourbar

        robot::moveFourbar(robot::fourbarVelocity);

        pros::delay(fbM);
        
        robot::moveFourbar(0);


        // Moving intake

        robot::moveIntake(robot::intakeVelocity);

        pros::delay(rmM);

        robot::moveIntake(0);
    }
}