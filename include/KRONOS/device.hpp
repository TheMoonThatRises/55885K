#include <string>
#include <vector>

#ifndef _KRONOS_DEVICE_HPP_
#define _KRONOS_DEVICE_HPP_

namespace KRONOS {
    template<class D>
    class Device {
        private:
            std::vector<std::string> reserved;
        public:
            Device(D device, std::string name, std::string callsign);
            D device;
            std::string name;
            std::string callsign;
    };
}

#endif