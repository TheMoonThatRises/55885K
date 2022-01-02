#include "helpers.h"

#ifndef _KRONOS_CONTROLLER_HPP_
#define _KRONOS_CONTROLLER_HPP_

class Controller: public pros::Controller {
    public:
        explicit Controller(pros::controller_id_e_t controller);
        void setControllerText(const std::string& text);
};

#endif