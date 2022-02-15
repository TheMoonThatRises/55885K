#include "vision.hpp"

using namespace KRONOS;

Vision::Vision(const int8_t& port):
    pros::Vision(port)
{

}

void Vision::addSignature(const int& name, const pros::vision_signature_s_t& signature) {
    signatures.insert({name, signature});
}

pros::vision_signature_s_t& Vision::getSignature(const int& name) {
    return signatures.at(name);
}

void Vision::setSignature(const int& name) {
    set_signature(name, &(signatures.at(name)));
}