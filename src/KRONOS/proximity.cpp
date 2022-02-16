#include "proximity.hpp"

using namespace KRONOS;

Proximity::Proximity(const int8_t& port):
    pros::Distance(port)
{

}