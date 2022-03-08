#include "helpers.h"
#include "robot.hpp"

#ifndef _KRONOS_CONTROLLER_HPP_
#define _KRONOS_CONTROLLER_HPP_

namespace KRONOS {
    class Controller: public pros::Controller {
        private:
            /*
                Adds callsigns to controller controls.

                @param map
                @param control Control you want to link.
                @param callsigns List of callsigns.
            */
            template<class T>
            Controller& link(std::map<T, std::map<std::string, function>>& linker, const T& control, function action, function idle);
        protected:
            Robot& robot;
            std::map<pros::controller_digital_e_t, std::map<std::string, function>> digitalLink;
            std::map<pros::controller_analog_e_t, std::map<std::string, function>> analogLink;
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
                @param action Function to run.
                @return Allows you to train functions together.
            */
            Controller& linkDigital(const pros::controller_digital_e_t& control, const function& action, const function& idle);

            /*
                Links the joystick value to a device.
                @param control Joystick and axis to link.
                @param action Function to run.
                @return Allows you to train functions together. 
            */
            Controller& linkAnalog(const pros::controller_analog_e_t& control, const function& action, const function& idle);
            void listener();
    };
}

#endif