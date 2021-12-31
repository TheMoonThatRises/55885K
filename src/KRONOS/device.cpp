#include "device.hpp"
#include <string>

using namespace KRONOS;

template<class D>
Device<D>::Device(D device, std::string name, std::string callsign):
    device(device),
    name(name),
    callsign(callsign)
{

}