#include "helpers.h"

#ifndef _KRONOS_PISTON_HPP_
#define _KRONOS_PISTON_HPP_

namespace KRONOS {
    class Piston: public pros::ADIDigitalOut {
        protected:
            bool value;
        public:
            explicit Piston(char port);
            bool set_value(bool value);
            bool get_value();
    };
}

#endif