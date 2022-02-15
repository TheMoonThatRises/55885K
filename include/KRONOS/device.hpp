#include "helpers.h"

#ifndef _KRONOS_DEVICE_HPP_
#define _KRONOS_DEVICE_HPP_

namespace KRONOS {
    enum device_types {
        MOTOR,
        PISTON,
        PROXIMITY,
        VISION
    };
    
    template<class D>
    class Device {
        private:
            const std::vector<std::string> reserved;
        public:
            /*
                @param device
                @param name A string name unique to the device.
                @param callsign A two letter combination unique to the device used mainly for auton.
            */
            Device(const D& device, const std::string& name, const std::string& callsign);
            D device;
            std::string name;
            std::string callsign;
    };
}

#endif