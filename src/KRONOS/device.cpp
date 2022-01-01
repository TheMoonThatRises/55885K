#include "device.hpp"
#include <stdexcept>
#include <algorithm>

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ " (" S2(__LINE__) ")"

using namespace KRONOS;

template<class D>
Device<D>::Device(D device, std::string name, std::string callsign):
    reserved {

    },
    device(device),
    name(name),
    callsign(callsign)
{
    if (callsign.length() != 2) throw std::runtime_error("Callsign must be length 2. " + std::string(LOCATION));
    else if (name.length() < 3) throw std::runtime_error("Name must be length 3 or longer. " + std::string(LOCATION));
    
    if (std::find(reserved.begin(), reserved.end(), callsign) != reserved.end()) throw std::runtime_error(callsign + " is a reserved sign. " + std::string(LOCATION));
}