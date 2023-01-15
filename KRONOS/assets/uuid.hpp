/*
  @file assets/uuid.hpp

  Stores class to generate UUIDs
*/

#ifndef _UUID_HPP_
#define _UUID_HPP_

#include <random>

namespace KUUID {
  class UUIDGenerator {
    private:
      const std::string _characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
      const int _max_characters = 12;

      std::random_device _rd; // obtain a random number from hardware
      std::mt19937 _gen; // seed the generator
      std::uniform_int_distribution<> _distr; // define the range
    public:
      explicit UUIDGenerator() : _gen(_rd()), _distr(0, (int) _characters.size() - 1) {};

      std::string generate_uuid() {
        std::string id;

        for (int i = 0; i < _max_characters; ++i) {
          id += _characters[_distr(_gen)];

          if (i % 4 == 0) {
            id += "-";
          }
        }

        return id;
      }
  };
}

#endif