#include "helpers.h"

#ifndef _KRONOS_DEVICE_HPP_
#define _KRONOS_DEVICE_HPP_

namespace KRONOS {
    enum device_types {
        MOTOR,
        PISTON
    };
    
    template<class D>
    class Device {
        private:
            const std::vector<std::string> reserved;
        public:
            Device(const D& device, const std::string& name, const std::string& callsign);
            D device;
            std::string name;
            std::string callsign;
    };
}

#endif