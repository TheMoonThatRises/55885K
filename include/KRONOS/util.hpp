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

        /*
            @param str
            @param num
        */
        static std::string addNumberString(const std::string& str, const int& num);

        /*
            @param str
            @param num
        */
        static std::string addNumberString(const std::string& str, const std::string& num);
};

#endif