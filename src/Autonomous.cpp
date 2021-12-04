#include "main.h"
#include "Autonomous.h"
#include "ClassHolders.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


Autonomous::Autonomous():
    selectAutonButton('A'),
    lockinAutonButton('B')

{
    std::cout << "Successfully initialized Autonomous" << std::endl;
}

void Autonomous::selectAuton() {
    bool isSelecting = true;

    while (isSelecting) {
        if (selectAutonButton.get_value()) {
            util.auton = (util.auton >= util.Auton.size() - 1) ? 0 : util.auton + 1;

            std::cout << util.Auton.size() << std::endl;

            controller.setControllerText("Auton < " + util.autonToString.at(util.auton));

            pros::delay(200);
        } else if (lockinAutonButton.get_value()) isSelecting = false;
    }

    controller.setControllerText("Locked < " + util.autonToString.at(util.auton));
}


/**
 * The commands are:
 *      ls = Left chassis side
 *      rs = Right chassis side
 *      cT = Chassis move length - Required if ls or rs is used
 *      fb = Fourbar
 *      fT = Fourbar move length - Required if fb is used
 *      bf = Back fourbar
 *      bT = Back fourbar move length - Required if bf is used
 **/

void Autonomous::loadRunFile(const std::string& autonString) {
    std::string commands;
    std::istringstream Commands(autonString);

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = util.splitString(commands, "_");
        int lsM = 0, rsM = 0, lrT = 0, fbM = 0, fbT = 0, bfb = 0, bfbT = 0;

        for (const std::string& command : commandAr) {
            std::cout << command.substr(2) << " : " << command << std::endl;
            int dist = std::stoi(command.substr(2));
            std::string commandSt = command.substr(0, 2);
            
            if (commandSt == "ls") lsM = dist;
            else if (commandSt == "rs") rsM = dist;
            else if (commandSt == "cT") lrT = dist;
            else if (commandSt == "fb") fbM = dist;
            else if (commandSt == "fT") fbT = dist;
            else if (commandSt == "bf") bfb = dist;
            else if (commandSt == "bT") bfbT = dist;
        }


        // Move chassis

        robot.moveChassis(lsM, rsM, 0);

        pros::delay(lrT);

        robot.moveChassis(0, 0, 0);


        // Move fourbar

        robot.moveFourbar(robot.fourbarL, robot.fourbarR, fbM);

        pros::delay(fbT);

        robot.moveFourbar(robot.fourbarL, robot.fourbarR, 0);

        // Move back fourbar

        robot.moveFourbar(robot.backFourbarL, robot.backFourbarR, bfb);

        pros::delay(bfbT);

        robot.moveFourbar(robot.backFourbarL, robot.backFourbarR, 0);
    }
}