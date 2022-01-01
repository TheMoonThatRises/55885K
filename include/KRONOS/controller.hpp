#include "main.h"
#include <string>

#ifndef _KRONOS_CONTROLLER_HPP_
#define _KRONOS_CONTROLLER_HPP_

class Controller: public pros::Controller {
    public:
        Controller(pros::controller_id_e_t controller);
        void setControllerText(std::string text);
};

#endif