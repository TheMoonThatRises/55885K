#include "piston.hpp"

using namespace KRONOS;

Piston::Piston(const char port):pros::ADIDigitalOut(port) {

}

bool Piston::set_value(bool setValue) {
    pros::ADIDigitalOut::set_value(setValue);
    value = setValue;

    return value;
}

bool Piston::get_value() {
    return value;
}