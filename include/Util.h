#include <string>
#include <vector>

#ifndef _UTIL_H_
#define _UTIL_H_

class Util {
    public:
        Util();

        const std::vector<std::string> autonToString,
                                       Auton;
        int auton;
        static std::vector<std::string> splitString(const std::string& str,const std::string& sep);
};

#endif