#include "helpers.h"
#include "robot.hpp"

#ifndef _KRONOS_CONTROLLER_HPP_
#define _KRONOS_CONTROLLER_HPP_

namespace KRONOS {
    class Controller: public pros::Controller {
        private:
            template<class T>
            Controller& link(std::map<T, std::vector<std::string>>& linker, const T& control, const std::vector<std::string>& callsigns);
        protected:
            Robot& robot;
            std::map<pros::controller_digital_e_t, std::vector<std::string>> digitalLink;
            std::map<pros::controller_analog_e_t, std::vector<std::string>> analogLink;
        public:
            /*
                @param robot
            */
            explicit Controller(Robot& robot);

            /*
                Sets the text of the controller.
                @param text Text for the controller to display.
            */
            void setControllerText(const std::string& text);

            /*
                Links one of the buttons do an action.
                @param control Button to link.
                @param callsign The callsign of a device or device pair.
                @param isPair If the callsign is a device pair or not.
                @return Allows you to train functions together.
            */
            Controller& linkDigital(const pros::controller_digital_e_t& control, const std::string& callsign, const bool& isPair);

            /*
                Links the joystick value to a device.
                @param control Joystick and axis to link.
                @param callsign The callsign of a device or device pair.
                @param isPair If the callsign is a device pair or not.
                @return Allows you to train functions together. 
            */
            Controller& linkAnalog(const pros::controller_analog_e_t& control, const std::string& callsign, const bool& isPair);
            void listener();
    };
}

#endif