#include "util.hpp"
#include <fstream>

// Code from stack answer https://stackoverflow.com/a/16286297
std::vector<std::string> Util::splitString(const std::string& str,const std::string& sep) {
    char* cstr = const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current = std::strtok(cstr,sep.c_str());
    while(current != nullptr){
        arr.emplace_back(current);
        current = std::strtok(nullptr,sep.c_str());
    }
    return arr;
}