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
            explicit Controller(Robot& robot);
            void setControllerText(const std::string& text);
            Controller& linkDigital(const pros::controller_digital_e_t& control, const std::string& callsign, const bool& isPair);
            Controller& linkAnalog(const pros::controller_analog_e_t& control, const std::string& callsign, const bool& isPair);
            void listener();
    };
}

#endif