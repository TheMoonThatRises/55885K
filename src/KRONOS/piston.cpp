#include "piston.hpp"

using namespace KRONOS;

Piston::Piston(const char& port):
    pros::ADIDigitalOut(port),
    value(false)
{

}

bool Piston::set_value(bool setValue) {
    pros::ADIDigitalOut::set_value(setValue);
    value = setValue;

    return value;
}

bool Piston::toggle() {
    set_value(!value);

    return value;
}

bool Piston::get_value() const {
    return value;
}