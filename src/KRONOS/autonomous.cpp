#include "autonomous.hpp"
#include "util.hpp"

using namespace KRONOS;

Autonomous::Autonomous(Robot& robot, Controller& controller, const std::vector<std::string>& autons, const std::vector<std::string>& autonToString):
    robot(robot),
    controller(controller),
    autons (autons),
    autonToString (autonToString),
    auton(0)
{

}

void Autonomous::selectAuton() {
    bool isSelected = false;

    while (!isSelected) {
        if (robot.getButton("select").get_value()) {
            auton = (auton >= autons.size() - 1) ? 0 : auton + 1;

            std::cout << autons.size() << std::endl;

            controller.setControllerText("Auton < " + autonToString.at(auton));
            pros::delay(200);
        } else if (robot.getButton("lock").get_value()) 
            isSelected = true;
    }

    controller.setControllerText("Locked < " + autonToString.at(auton));
}

void Autonomous::runAuton() {
    auto findDevices = [&](const std::string& command, const int& speed) {
        try {
            robot.getMotor(command).move_velocity(speed);
        } catch (std::runtime_error err) {
            try {
                robot.movePairMotors(command, speed);
            } catch (std::runtime_error err2) {
                try {
                    if (speed != 0)
                        robot.getPiston(command).toggle();
                } catch (std::runtime_error err3) { }
            }
        }
    };

    std::string commands;
    std::istringstream Commands(autons.at(auton));

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = Util::splitString(commands, "_");
        std::map<std::string, int> dists {};

        for (const std::string& command : commandAr) {
            int dist = (!command.substr(2).empty()) ? std::stoi(command.substr(2)) : 0;
            std::string commandSt = command.substr(0, 2);
            
            dists.insert({commandSt, dist});
        }

        for (const auto& [command, speed] : dists)
            if (command != "ln")
                findDevices(command, speed);

        if (dists.find("ln") != dists.end()) {
            pros::delay(dists.at("ln"));

            for (const auto& [command, speed] : dists)
                if (command != "ln")
                findDevices(command, 0);
        }
    }
}