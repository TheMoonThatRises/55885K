#include "main.h"


#ifndef _KRONOS_MOTOR_HPP_
#define _KRONOS_MOTOR_HPP_

namespace KRONOS {
    class Motor: public pros::Motor {
        public:
            Motor(int8_t port, pros::motor_gearset_e_t gearset, bool reverse, pros::motor_encoder_units_e_t encoder);
            void curve_velocity(int32_t velocity);
            void curve_relative(int32_t velocity, double distance);
    };
}

#endif