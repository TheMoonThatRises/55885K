#include <string>

#ifndef _KRONOS_DEVICE_HPP_
#define _KRONOS_DEVICE_HPP_

namespace KRONOS {
    template<class D>
    class Device {
        public:
            Device(D device, std::string name, std::string callsign);
            D device;
            std::string name;
            std::string callsign;
    };
}

#endif