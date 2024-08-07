/*
  Copyright 2024 Peter Duanmu

  @file base/managers/varmanager.hpp

  Stores variables to a key
*/

#ifndef KRONOS_BASE_MANAGERS_VARMANAGER_HPP_
#define KRONOS_BASE_MANAGERS_VARMANAGER_HPP_

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "internal/hotp.hpp"

namespace KRONOS {
class VarManager {
 private:
  class Variable {
   private:
    std::unique_ptr<std::any> _value;
    bool _is_elevated;

    friend VarManager;

    template <class T>
    inline void update_value(const T &value) {
      _value = std::make_unique<std::any>(std::make_any<T>(value));
    }

    inline std::any* get_value() {
      return _value.get();
    }

   public:
    template <class T>
    explicit Variable(const T& value, const bool &is_elevated)
    : _value(std::make_unique<std::any>(std::make_any<T>(value))),
      _is_elevated(is_elevated) {}

    ~Variable() {
      _value.reset(nullptr);
    }
  };

  std::map<std::string, std::unique_ptr<Variable>> _global {};
  KOTP::HOTP _htop;

  inline bool var_exists(const std::string &key) {
    return _global.find(key) != _global.end();
  }

  inline bool is_elevated(const std::string &key) {
    return _global.at(key)->_is_elevated;
  }

 public:
    explicit VarManager(const std::string &secret_key) : _htop(secret_key) {}

    template <class T>
    inline void global_set(
      const std::string &key,
      const T& value,
      const int &otp = 0) {
      bool is_verified = _htop.verify(otp);

      if (var_exists(key)) {
        if (is_elevated(key) && !is_verified) {
          throw new InvalidElevatedVariableAccess(key);
        }

        KLog::Log::info("setting elevated variable '" + key + "'");

        _global.at(key)->update_value(value);
      } else {
        (void) _global.insert(std::make_pair(
          key,
          std::make_unique<Variable>(value, is_verified)));
      }
    }

    template<class T>
    inline T* global_get(const std::string &key) {
      return var_exists(key)
        ? std::any_cast<T>(_global.at(key)->get_value())
        : nullptr;
    }

    inline void global_delete(const std::string &key, const int &otp = 0) {
      if (is_elevated(key) && !_htop.verify(otp)) {
        throw new InvalidElevatedVariableAccess(key);
      }

      KLog::Log::info("deleting elevated variable '" + key + "'");

      _global.at(key)->~Variable();
      _global.at(key).reset(nullptr);
      (void) _global.erase(key);
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_VARMANAGER_HPP_
