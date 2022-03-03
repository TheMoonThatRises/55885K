#include "helpers.h"

#ifndef _KRONOS_UTIL_HPP_
#define _KRONOS_UTIL_HPP_

class Util {
    public:
        /*
            @param str
            @param sep
            @return
        */
        static std::vector<std::string> splitString(const std::string& str, const std::string& sep);
        static std::string addNumberString(const std::string& str, const int& num);
};

#endif