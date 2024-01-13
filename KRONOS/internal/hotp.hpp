/*
  Copyright 2024 Peter Duanmu

  @file internal/hotp.hpp

  HMAC One Time Password
*/

#ifndef KRONOS_INTERNAL_HOTP_HPP_
#define KRONOS_INTERNAL_HOTP_HPP_

#include <string>

#include "external/hashlibrary/hmac.hpp"
#include "external/hashlibrary/sha256.hpp"

namespace KOTP {
class HOTP {
 private:
  std::string _secret_key;
  inline static size_t _counter = 0;

 public:
  explicit HOTP(const std::string &secret_key)
  : _secret_key(secret_key) {}

  inline int next_code() const {
    std::string hash = hmac<SHA256>(std::to_string(_counter), _secret_key);

    size_t offset = hash[19] & 0xf;

    int truncated =
      (hash[offset] & 0x7f) << 24 |
      (hash[offset + 1] & 0xff) << 16 |
      (hash[offset + 2] & 0xff) << 8 |
      (hash[offset + 3] & 0xff);

    return truncated;
  }

  inline bool verify(const int &code) const {
    bool verified = code == next_code();

    ++_counter;

    return verified;
  }
};
}  // namespace KOTP

#endif  // KRONOS_INTERNAL_HOTP_HPP_
