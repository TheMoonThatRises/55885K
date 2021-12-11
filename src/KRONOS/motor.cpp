#include "motor.hpp"

using namespace KRONOS;

Motor::Motor(const int8_t port, const pros::motor_gearset_e_t gearset, const bool reverse, const pros::motor_encoder_units_e_t encoder):pros::Motor(port, gearset, reverse, encoder) {

}

void Motor::curve_velocity(int32_t velocity) {
    
}

void Motor::curve_relative(int32_t velocity, double distance) {
    
}