#include "helpers.h"

#ifndef _KRONOS_MOTOR_HPP_
#define _KRONOS_MOTOR_HPP_

namespace KRONOS {
    class Motor: public pros::Motor {
        public:
            /*
                @param port
                @param gearset
                @param reverse
                @param encoder
                @param brakemode
            */
            Motor(const int8_t& port, const pros::motor_gearset_e_t& gearset, const bool& reverse, const pros::motor_encoder_units_e_t& encoder, const pros::motor_brake_mode_e_t& brakemode);

            /*
                @param velocity
            */
            void curve_velocity(const int32_t& velocity);

            /*
                Makes the motor speed to the target velocity and slows down to the target distance.

                @param velocity
                @param distance
            */
            void curve_relative(const int32_t& velocity, const double& distance);
    };
}

#endif