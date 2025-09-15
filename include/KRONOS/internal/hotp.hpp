/*
  Copyright 2025 Peter Duanmu

  @file internal/hotp.hpp

  HMAC One Time Password
*/

#ifndef KRONOS_INTERNAL_HOTP_HPP_
#define KRONOS_INTERNAL_HOTP_HPP_

#include <string>

namespace kronos::internal {
class HOTP {
 private:
  std::string _secret_key;
  inline static size_t _counter = 0;

 public:
  explicit HOTP(const std::string& secret_key);

  int get_code() const;
  bool verify(const int& code) const;
};
}  // namespace kronos::internal

#endif  // KRONOS_INTERNAL_HOTP_HPP_
