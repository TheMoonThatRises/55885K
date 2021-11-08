#include <string>

class util {
    public:
        static std::string brakeToString[],
                           modeToString[];
        static void testWheels();
        static void testFourbar();
        static void testAll();
        static void calibrateFourbar();
        static void autonomous();
};