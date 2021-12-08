#include "Util.h"
#include "ClassHolders.h"
#include <string>
#include <vector>
#include <fstream>


Util::Util():
        Auton{
            "fb150_fT500_fT500\nfb-150_fT500\nls200_rs200_cT1500\nfb180_fT800\nls-100_rs-100_cT4000",
            ""
        },
        autonToString{"midGoals", "None"},
        auton(0)
{
    std::cout << "Successfully initialized Util" << std::endl;
}

// Code from stack answer https://stackoverflow.com/a/16286297
std::vector<std::string> Util::splitString(const std::string& str,const std::string& sep){
    char* cstr = const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current = strtok(cstr,sep.c_str());
    while(current != nullptr){
        arr.emplace_back(current);
        current = strtok(nullptr,sep.c_str());
    }
    return arr;
}