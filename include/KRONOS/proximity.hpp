#include "helpers.h"
#include "motor.hpp"
#include "vision.hpp"

#ifndef _KRONOS_PROXIMITY_HPP_
#define _KRONOS_PROXIMITY_HPP_

namespace KRONOS {
    class Proximity : public pros::Optical {
        public:
            /*
                @param port
            */
            Proximity(const int8_t& port);
    };
}

#endif