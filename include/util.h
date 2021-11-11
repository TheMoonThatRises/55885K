#include <string>
#include <vector>
#include <map>

class util {
    public:
        static std::string brakeToString[],
                           modeToString[],
                           autonToString[],
                           Auton[];
        static int auton;
        static void testWheels();
        static void testFourbar();
        static void testAll();
        static bool checkOverheat();
        static void calibrateFourbar();
        static std::string fileToAuton(std::string inputFile);
        static std::vector<std::string> splitString(std::string str,std::string sep);
};