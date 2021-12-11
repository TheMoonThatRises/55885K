#include "main.h"


#ifndef _KRONOS_PISTON_HPP_
#define _KRONOS_PISTON_HPP_

namespace KRONOS {
    class Piston: public pros::ADIDigitalOut {
        protected:
            bool value;
        public:
            Piston(const char port);
            bool set_value(const bool value);
            bool get_value();
    };
}

#endif