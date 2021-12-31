#include "main.h"
#include "autonomous.hpp"
#include "robot.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


using namespace KRONOS;

Autonomous::Autonomous(Robot robot):
    robot(robot),
    selectAutonB(robot.getButton("select")),
    lockAutonB(robot.getButton("lock")),
    autons {
            "ls200_rs200_cT1500\ncl\nls-100_rs-100_cT3000\ncl",
            ""
    },
    autonToString {"midGoals", "None"},
    auton(0)
{
    
}

void Autonomous::selectAuton() {
    bool isSelected = false;

    while (!isSelected) {
        if (selectAutonB.get_value()) {
            auton = (auton >= autons.size() - 1) ? 0 : auton + 1;

            std::cout << autons.size() << std::endl;

            // robot.getController().setControllerText("Auton < " + autonToString.at(auton));
            pros::delay(200);
        } else if (lockAutonB.get_value()) isSelected = true;
    }

    // controller.setControllerText("Locked < " + autonToString.at(auton));
}

void Autonomous::loadRunString() {
    std::string commands;
    std::istringstream Commands(autons.at(auton));

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = Util::splitString(commands, "_");
        int lsM = 0, rsM = 0, lrT = 0, fbM = 0, fbT = 0, cP = 0, bk = 0, bkT = 0;
        bool moveClaw = false;

        for (const std::string& command : commandAr) {
            std::cout << command.substr(2) << " : " << command << std::endl;
            int dist = (!command.substr(2).empty()) ? std::stoi(command.substr(2)) : 0;
            std::string commandSt = command.substr(0, 2);
            
            if (commandSt == "ls") lsM = dist;
            else if (commandSt == "rs") rsM = dist;
            else if (commandSt == "cT") lrT = dist;
            else if (commandSt == "fb") fbM = dist;
            else if (commandSt == "fT") fbT = dist;
            else if (commandSt == "cl") moveClaw = true;
            else if (commandSt == "cP") cP = dist;
            else if (commandSt == "bk") bk = dist;
            else if (commandSt == "bT") bkT = dist;
        }

        
    }
}