#include "autonomous.hpp"
#include "util.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>


using namespace KRONOS;

Autonomous::Autonomous(Robot &robot):
    robot(robot),
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
        if (robot.getButton("select").get_value()) {
            auton = (auton >= autons.size() - 1) ? 0 : auton + 1;

            std::cout << autons.size() << std::endl;

            robot.getController().setControllerText("Auton < " + autonToString.at(auton));
            pros::delay(200);
        } else if (robot.getButton("lock").get_value()) isSelected = true;
    }

    robot.getController().setControllerText("Locked < " + autonToString.at(auton));
}

void Autonomous::runAuton() {
    std::string commands;
    std::istringstream Commands(autons.at(auton));

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = Util::splitString(commands, "_");
        std::map<std::string, int> dists {};

        for (const std::string command : commandAr) {
            std::cout << command.substr(2) << " : " << command << std::endl;
            int dist = (!command.substr(2).empty()) ? std::stoi(command.substr(2)) : 0;
            std::string commandSt = command.substr(0, 2);
            
            dists.insert({commandSt, dist});
        }

        for (const auto [command, speed] : dists)
            if (command != "ln")
                try {
                    robot.getMotor(command).move_velocity(speed);
                } catch (std::runtime_error err) {
                    try {
                        robot.getPiston(command).set_value(!robot.getPiston(command).get_value());
                    } catch (std::runtime_error err2) { }
                }

        if (dists.at("ln") && std::isdigit(dists.at("ln"))) pros::delay(dists.at("ln"));
    }
}