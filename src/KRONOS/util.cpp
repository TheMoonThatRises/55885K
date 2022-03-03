#include "util.hpp"

// Code from stack answer https://stackoverflow.com/a/16286297
std::vector<std::string> Util::splitString(const std::string& str,const std::string& sep) {
    char* cstr = const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current = strtok(cstr, sep.c_str());
    while(current != nullptr){
        arr.emplace_back(current);
        current = strtok(nullptr, sep.c_str());
    }
    return arr;
}

std::string Util::addNumberString(const std::string& str, const int& num) {
    return std::to_string(std::stoi(str) + num);
}