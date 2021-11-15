#include <string>
#include <vector>

#ifndef _UTIL_H_
#define _UTIL_H_

class util {
    public:
        static const std::vector<std::string> modeToString,
                                        autonToString,

                                        Auton;
        static int auton;
        static void testWheels();
        static void testFourbar();
        static void testAll();
        static bool checkOverheat();
        static void calibrateFourbar();
        static std::string fileToAuton(const std::string& inputFile);
        static std::vector<std::string> splitString(const std::string& str,const std::string& sep);
};

#endif