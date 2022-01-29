#include "helpers.h"

#ifndef _KRONOS_PISTON_HPP_
#define _KRONOS_PISTON_HPP_

namespace KRONOS {
    class Piston: public pros::ADIDigitalOut {
        protected:
            bool value;
        public:
            /*
                @param port
            */
            explicit Piston(const char& port);

            /*
                @param setValue
            */
            bool set_value(bool setValue);
            bool toggle();
            bool get_value() const;
    };
}

#endif