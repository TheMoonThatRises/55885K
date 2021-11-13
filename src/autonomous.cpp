#include "main.h"
#include "autonomous.h"
#include "robot.h"
#include "util.h"
#include "controller.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


pros::ADIDigitalIn autonomous::selectAutonButton('A'),
                   autonomous::lockinAutonButton('B');

void autonomous::selectAuton() {
    bool isSelecting = true;

    while (isSelecting) {
        if (autonomous::selectAutonButton.get_value()) {
            util::auton = (util::auton + 1) % util::Auton.size();

            std::cout << util::Auton.size() << std::endl;

            controller::setControllerText("Auton < " + util::autonToString.at(util::auton));

            pros::delay(200);
        } else if (autonomous::lockinAutonButton.get_value()) isSelecting = false;
    }

    controller::setControllerText("Locked < " + util::autonToString.at(util::auton));
}


/**
 * The commands are:
 *      ls = Left chassis side
 *      rs = Right chassis side
 *      cT = Chassis move length - Required if ls or rs is used
 *      fb = Fourbar
 *      fT = Fourbar move length - Required if fb is used
 *      rm = Ring motor
 *      rT = Ring motor move length - Required if rm is used
 **/

void autonomous::loadRunFile(const std::string& auton) {
    std::string commands;
    std::istringstream Commands(auton);

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = util::splitString(commands, "_");
        int lsM = 0, rsM = 0, lrT = 0, fbM = 0, fbT = 0, rmM = 0, rmT = 0;

        for (const std::string& command : commandAr) {
            std::cout << command.substr(2) << " : " << command << std::endl;
            int dist = std::stoi(command.substr(2));
            std::string commandSt = command.substr(0, 2);
            
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
            } else if (commandSt == "rT") {
                rmT = dist;
            } else if (commandSt == "fT") {
                fbT = dist;
            }
        }


        // Moving chassis

        robot::moveChassis(lsM, rsM, 0);

        pros::delay(lrT);

        robot::moveChassis(0, 0, 0);


        // Moving fourbar

        robot::moveFourbar(fbM);

        pros::delay(fbT);

        robot::moveFourbar(0);


        // Moving intake

        robot::moveIntake(rmM);

        pros::delay(rmT);

        robot::moveIntake(0);
    }
}