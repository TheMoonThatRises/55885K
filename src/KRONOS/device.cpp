#include "device.hpp"
#include <string>
#include <stdexcept>

using namespace KRONOS;

template<class D>
Device<D>::Device(D device, std::string name, std::string callsign):
    reserved {

    },
    device(device),
    name(name),
    callsign(callsign)
{
    if (callsign.length() != 2) throw std::runtime_error("Callsign must be length 2. " + __FILE__ + " (" + __LINE__ + ")");
    else if (name.length() < 3) throw std::runtime_error("Name must be length 3 or longer. " + __FILE__ + " (" + __LINE__ + ")");
    
    for (std::string reserve : reserved) if (callsign == reserve) throw std::runtime_err(callsign + " is a reserved sign. " + __FILE__ + " (" + __LINE__ + ")")
}