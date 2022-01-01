#include "device.hpp"
#include <stdexcept>
#include <algorithm>

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
    
    if (std::find(reserved.begin(), reserved.end(), callsign) != reserved.end()) throw std::runtime_err(callsign + " is a reserved sign. " + __FILE__ + " (" + __LINE__ + ")")
}