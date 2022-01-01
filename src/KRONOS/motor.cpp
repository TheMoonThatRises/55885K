#include "motor.hpp"
#include <stdexcept>

using namespace KRONOS;

Motor::Motor(const int8_t port, const pros::motor_gearset_e_t gearset, const bool reverse, const pros::motor_encoder_units_e_t encoder, pros::motor_brake_mode_e brakemode):pros::Motor(port, gearset, reverse, encoder) {
    set_brake_mode(brakemode);
}

void Motor::curve_velocity(int32_t velocity) {
    int8_t currentVelocity = get_actual_velocity();
    double addVelocity = 5;
    double sleepTime = 2;

    auto whileCurve = [&](bool greater) {
        move_velocity(greater ? -addVelocity : addVelocity);
        pros::delay(sleepTime);
        sleepTime /=1.7;
        addVelocity *= 1.5;
    };

    if (currentVelocity > velocity) while (currentVelocity > velocity) whileCurve(true);
    else if (currentVelocity < velocity) while (currentVelocity < velocity) whileCurve(false);
}

void Motor::curve_relative(int32_t velocity, double distance) {
    
}