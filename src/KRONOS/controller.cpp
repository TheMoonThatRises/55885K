#include "main.h"
#include "controller.hpp"
#include <iostream>

Controller::Controller(pros::controller_id_e_t controller):pros::Controller(controller) {

}

void Controller::setControllerText(std::string text) {
    set_text(0, 0, text + "         ");

    std::cout << text << std::endl;
}