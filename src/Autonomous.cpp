#include "main.h"
#include "Autonomous.h"
#include "ClassHolders.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


Autonomous::Autonomous():
    selectAutonB('B'),
    lockAutonB('C')
{
    std::cout << "Successfully initialized Autonomous" << std::endl;
}

void Autonomous::selectAuton() {
    bool isSelected = false;

    while (!isSelected) {
        if (selectAutonB.get_value()) {
            util.auton = (util.auton >= util.Auton.size() - 1) ? 0 : util.auton + 1;

            std::cout << util.Auton.size() << std::endl;

            controller.setControllerText("Auton < " + util.autonToString.at(util.auton));
            pros::delay(200);
        } else if (lockAutonB.get_value()) isSelected = true;
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
 *      cl = Open / Close the claw
 *      cP = Wait time after claw - Not required
 **/

void Autonomous::loadRunString(const std::string& autonString) {
    std::string commands;
    std::istringstream Commands(autonString);

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = Util::splitString(commands, "_");
        int lsM = 0, rsM = 0, lrT = 0, fbM = 0, fbT = 0, cP = 0;
        bool moveClaw = false;

        for (const std::string& command : commandAr) {
            std::cout << command.substr(2) << " : " << command << std::endl;
            int dist = (command.substr(2) != "") ? std::stoi(command.substr(2)) : 0;
            std::string commandSt = command.substr(0, 2);
            
            if (commandSt == "ls") lsM = dist;
            else if (commandSt == "rs") rsM = dist;
            else if (commandSt == "cT") lrT = dist;
            else if (commandSt == "fb") fbM = dist;
            else if (commandSt == "fT") fbT = dist;
            else if (commandSt == "cl") moveClaw = true;
            else if (commandSt == "cP") cP = dist;
        }


        // Move chassis

        robot.moveChassis(lsM, rsM);

        pros::delay(lrT);

        robot.moveChassis(0, 0);


        // Move fourbar

        robot.moveFourbar(fbM);

        pros::delay(fbT);

        robot.moveFourbar(0);

        // Open / Close claw

        if (moveClaw) {
            robot.clawPistonValue = !robot.clawPistonValue;
            robot.clawPiston.set_value(robot.clawPistonValue);

            pros::delay(cP);
        }
    }
}