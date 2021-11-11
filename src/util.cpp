#include "util.h"
#include "robot.h"
#include "controller.h"
#include <string>
#include <vector>
#include <fstream>


const std::string util::Auton[] = {
    "ls100_rs100_cT200\nls100_rs-100_cT1500\nls-50_rs-50_cT1750\nrm-75_rT2000", 
    "fb100_fT500\nfb-100_fT1000\nls100_rs100_cT3000\nfb180_fT800\nls-50_rs-50_cT3000"
};

std::string util::brakeToString[] = {"Coast", "Brake", "Hold"},
            util::modeToString[] = {"Tank", "Single"},
            util::autonToString[] = {"duoGoals", "midGoals"};

int util::auton = 1;

void util::testWheels() {
    robot::moveChassis(robot::wheelAddedVelocity, robot::wheelAddedVelocity, 5000, 5000, 0);

    pros::delay(2000);

    robot::moveChassis(robot::wheelAddedVelocity, robot::wheelAddedVelocity, -5000, -5000, 0);
}

void util::testFourbar() {
    robot::moveFourbar(robot::fourbarVelocity, robot::fourbarMaxDistance);

    pros::delay(2000);

    robot::moveFourbar(robot::fourbarVelocity, -robot::fourbarMaxDistance);
}

void util::testAll() {
    util::testWheels();

    while (!robot::didWheelsStop()) {
        pros::delay(20);
    }

    util::testFourbar();
}

bool util::checkOverheat() {
    if (robot::RB.is_over_temp() || robot::RB.is_over_temp() || robot::LF.is_over_temp() || robot::LB.is_over_temp() || robot::fourbarL.is_over_temp() || robot::fourbarR.is_over_temp() || robot::ringMotor.is_over_temp()) return true;
    else return false;
}

void util::calibrateFourbar() {

}

std::string util::fileToAuton(std::string inputFile) {
    std::string commands;
    std::string inputCommands;

    std::ifstream CommandFile(inputFile);

    while (std::getline(CommandFile, commands)) {
        inputCommands += commands + "\n";
    }

    return inputCommands;
}

// Code from stack answer https://stackoverflow.com/a/16286297
std::vector<std::string> util::splitString(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}