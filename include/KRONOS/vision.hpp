#include "helpers.h"

#ifndef _KRONOS_VISION_HPP_
#define _KRONOS_VISION_HPP_

namespace KRONOS {
    class Vision : public pros::Vision {
        protected:
            std::map<int, pros::vision_signature_s_t> signatures;
        public:
            /*
                @param port
            */
            explicit Vision(const int8_t& port);

            /*
                Save a vision signature to the vision sensor.

                @param name
                @param signature
            */
            void addSignature(const int& name, const pros::vision_signature_s_t& signature);

            /*
                Get a vision signature saved from the sensor.

                @param name
            */
            pros::vision_signature_s_t& getSignature(const int& name);

            /*
                Set the signature for the vision sensor.

                @param name
            */
            void setSignature(const int& name);
    };
}

#endif