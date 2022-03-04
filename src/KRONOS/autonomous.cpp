#include "autonomous.hpp"
#include "util.hpp"

using namespace KRONOS;

Autonomous::Autonomous(Robot& robot, Controller& controller, const std::vector<std::string>& autons, const std::vector<std::string>& autonToString, int defaultAuton):
    robot(robot),
    controller(controller),
    autons (autons),
    autonToString (autonToString),
    auton(defaultAuton)
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
        } catch (std::exception& err) {
            try {
                robot.movePairMotors(command, speed);
            } catch (std::exception& err2) {
                try {
                    robot.getPiston(command).toggle();
                } catch (std::exception& err3) { }
            }
        }
    };

    std::string commands;
    std::istringstream Commands(autons.at(auton));

    while (std::getline(Commands, commands)) {
        std::vector<std::string> commandAr = Util::splitString(commands, "_");
        std::map<std::string, int> dists {};

        if (commandAr.at(0) == "tk") {
            std::vector<Motor> leftChassis = robot.getMotorPairs(commandAr.at(7)),
                               rightChassis = robot.getMotorPairs(commandAr.at(8));

            robot.followObject(robot.getVision(commandAr.at(1)), std::stoi(commandAr.at(2)), std::stoi(commandAr.at(3)), robot.getProximity(commandAr.at(4)), std::stoi(commandAr.at(5)), std::stoi(commandAr.at(6)), leftChassis, rightChassis, std::stoi(commandAr.at(9)), std::stoi(commandAr.at(10)));
        } else if (commandAr.at(0) == "bkt") {
            while (true) {
                if (std::stoi(commandAr.at(2)) >= robot.getProximity(commandAr.at(1)).get() && robot.getProximity(commandAr.at(1)).get()!= 0)
                    break;

                robot.movePairMotors(commandAr.at(3), std::stoi(commandAr.at(4)));
                robot.movePairMotors(commandAr.at(5), std::stoi(commandAr.at(6)));

                pros::delay(20);
            }

            robot.movePairMotors(commandAr.at(3), 0);
            robot.movePairMotors(commandAr.at(5), 0);
        } else if (commandAr.at(0) == "cbm") {
            for (Motor motor : robot.getMotorPairs(commandAr.at(1)))
                switch (std::stoi(commandAr.at(2))) {
                    case 0:
                        motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                        break;
                    case 1:
                        motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                        break;
                    case 2:
                        motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
                        break;
                }
        } else {
            for (const std::string& command : commandAr) {
                int dist = (!command.substr(2).empty()) ? std::stoi(command.substr(2)) : 0;
                std::string commandSt = command.substr(0, 2);

                dists.insert({commandSt, dist});
            }

            for (const auto& [command, speed] : dists)
                if (command != "tk" && command != "ln")
                    findDevices(command, speed);

            if (dists.find("ln") != dists.end()) {
                pros::delay(dists.at("ln"));

                for (const auto& [command, speed] : dists)
                    if (command != "tk" && command != "ln")
                        findDevices(command, 0);
            }
        }

        pros::delay(20);
    }
}