#include "helpers.h"

#ifndef _KRONOS_MOTOR_HPP_
#define _KRONOS_MOTOR_HPP_

namespace KRONOS {
    class Motor: public pros::Motor {
        public:
            Motor(const int8_t& port, const pros::motor_gearset_e_t& gearset, const bool& reverse, const pros::motor_encoder_units_e_t& encoder, const pros::motor_brake_mode_e_t& brakemode);
            void curve_velocity(const int32_t& velocity);
            void curve_relative(const int32_t& velocity, const double& distance);
    };
}

#endif