#include "device.hpp"
#include "piston.hpp"
#include "motor.hpp"

using namespace KRONOS;

template<class D>
Device<D>::Device(D device, const std::string& name, const std::string& callsign):
    reserved {
        "ln"
    },
    device(device),
    name(name),
    callsign(callsign)
{
    if (callsign.length() != 2) throw std::runtime_error("Callsign must be length 2. " + std::string(LOCATION));
    else if (name.length() < 3) throw std::runtime_error("Name must be length 3 or longer. " + std::string(LOCATION));
    
    if (std::find(reserved.begin(), reserved.end(), callsign) != reserved.end()) throw std::runtime_error(callsign + " is a reserved sign. " + std::string(LOCATION));
}

template class Device<Piston>;
template class Device<Motor>;