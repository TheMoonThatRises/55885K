/*
  Copyright 2025 Peter Duanmu

  @file assets/uuid.hpp

  Stores class to generate UUIDs
*/

#ifndef KRONOS_ASSETS_UUID_HPP_
#define KRONOS_ASSETS_UUID_HPP_

#include <random>
#include <string>

namespace kronos::assets {
class UUIDGenerator {
 private:
    const std::string _characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const int _max_characters = 12;

    std::random_device _rd;  // obtain a random number from hardware
    std::mt19937 _gen;  // seed the generator
    std::uniform_int_distribution<> _distr;  // define the range

 public:
    UUIDGenerator();

    std::string generate_uuid();
};
}  // namespace kronos::assets

#endif  // KRONOS_ASSETS_UUID_HPP_
