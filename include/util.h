#include <string>

class util {
    public:
        static std::string brakeToString[],
                           modeToString[];
        static void testWheels();
        static void testFourbar();
        static void testAll();
        static bool checkOverheat();
        static void calibrateFourbar();
};