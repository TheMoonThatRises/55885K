#include "Util.h"
#include "ClassHolders.h"
#include <string>
#include <vector>
#include <fstream>


Util::Util():
        Auton{
            "ls100_rs100_cT200\nls100_rs-100_cT1500\nls-50_rs-50_cT1750\nrm-75_rT2000",
            "fb150_fT500_fT500\nfb-150_fT500\nls200_rs200_cT1500\nfb180_fT800\nls-100_rs-100_cT4000",
            ""
        },
        modeToString{"Tank", "Single"},
        autonToString{"duoGoals", "midGoals", "None"},
        auton(1)
{
    std::cout << "Successfully initialized Util" << std::endl;
}

void Util::testWheels() {
    robot.moveChassis(robot.wheelAddedVelocity, robot.wheelAddedVelocity, 5000, 5000, 0);

    pros::delay(2000);

    robot.moveChassis(robot.wheelAddedVelocity, robot.wheelAddedVelocity, -5000, -5000, 0);
}

void Util::testFourbar() {
    robot.moveFourbar(robot.fourbarVelocity, robot.fourbarMaxDistance);

    pros::delay(2000);

    robot.moveFourbar(robot.fourbarVelocity, -robot.fourbarMaxDistance);

    pros::delay(2000);

    robot.moveBackFourbar(robot.fourbarVelocity, robot.fourbarMaxDistance);

    pros::delay(2000);

    robot.moveBackFourbar(robot.fourbarVelocity, -robot.fourbarMaxDistance);
}

void Util::testAll() {
    Util::testWheels();

    while (!robot.didWheelsStop()) {
        pros::delay(20);
    }

    Util::testFourbar();
}

bool Util::checkOverheat() {
    if (robot.RB.is_over_temp() || robot.RB.is_over_temp() || robot.LF.is_over_temp() || robot.LB.is_over_temp() || robot.fourbarL.is_over_temp() || robot.fourbarR.is_over_temp()) return true;
    else return false;
}

void Util::calibrateFourbar() {

}

std::string Util::fileToAuton(const std::string& inputFile) {
    std::string commands;
    std::string inputCommands;

    std::ifstream CommandFile(inputFile);

    while (std::getline(CommandFile, commands)) {
        inputCommands += commands + "\n";
    }

    return inputCommands;
}

// Code from stack answer https://stackoverflow.com/a/16286297
std::vector<std::string> Util::splitString(const std::string& str,const std::string& sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=nullptr){
        arr.emplace_back(current);
        current=strtok(nullptr,sep.c_str());
    }
    return arr;
}