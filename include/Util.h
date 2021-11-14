#include <string>
#include <vector>

#ifndef _UTIL_H_
#define _UTIL_H_

class Util {
    public:
        Util();

        const std::vector<std::string> modeToString,
                                       autonToString,

                                       Auton;
        int auton;
        void testWheels();
        void testFourbar();
        void testAll();
        bool checkOverheat();
        void calibrateFourbar();
        std::string fileToAuton(const std::string& inputFile);
        std::vector<std::string> splitString(const std::string& str,const std::string& sep);
};

#endif