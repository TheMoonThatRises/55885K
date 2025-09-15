/*
  Copyright 2025 Peter Duanmu

  @file internal/hotp.cpp

  HMAC One Time Password
*/

#include <string>

#include "KRONOS/external/hashlibrary/hmac.hpp"
#include "KRONOS/external/hashlibrary/sha256.hpp"

#include "KRONOS/internal/hotp.hpp"

namespace kronos::internal {
HOTP::HOTP(const std::string& secret_key)
: _secret_key(secret_key) {}

int HOTP::get_code() const {
  std::string hash = hmac<SHA256>(std::to_string(_counter), _secret_key);

  size_t offset = hash[19] & 0xf;

  int truncated =
    (hash[offset] & 0x7f) << 24 |
    (hash[offset + 1] & 0xff) << 16 |
    (hash[offset + 2] & 0xff) << 8 |
    (hash[offset + 3] & 0xff);

  return truncated;
}

bool HOTP::verify(const int& code) const {
  bool verified = code == get_code();

  ++_counter;

  return verified;
}
}  // namespace kronos::internal
