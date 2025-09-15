/*
  Copyright 2025 Peter Duanmu

  @file assets/uuid.cpp

  Stores class to generate UUIDs
*/

#include <string>

#include "KRONOS/assets/uuid.hpp"

namespace kronos::assets {
UUIDGenerator::UUIDGenerator()
: _gen(_rd()),
_distr(0, static_cast<int>(_characters.size()) - 1) {}

std::string UUIDGenerator::generate_uuid() {
  std::string id;

  for (int i = 0; i < _max_characters; ++i) {
    id += _characters[_distr(_gen)];

    if (i % 4 == 0) {
      id += "-";
    }
  }

  return id;
}
}  // namespace kronos::assets
