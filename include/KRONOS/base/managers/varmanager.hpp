/*
  Copyright 2025 Peter Duanmu

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

#include "KRONOS/internal/hotp.hpp"

namespace kronos::base::managers {
class VarManager {
 private:
  class Variable {
   private:
    std::unique_ptr<std::any> _value;
    bool _is_elevated;

    friend VarManager;

    template <class T>
    void update_value(const T& value);
    std::any* get_value();

   public:
    template <class T>
    explicit Variable(const T& value, const bool& is_elevated);

    ~Variable();
  };

  std::map<std::string, std::unique_ptr<Variable>> _global {};
  internal::HOTP _htop;

  bool var_exists(const std::string& key);
  bool is_elevated(const std::string& key);

 public:
    explicit VarManager(const std::string& secret_key);

    void global_set(
      const std::string& key,
      const std::any& value,
      const int& otp = 0);

    template<class T>
    inline T* global_get(const std::string& key) {
      return var_exists(key)
        ? std::any_cast<T>(_global.at(key)->get_value())
        : nullptr;
    }

    void global_delete(const std::string& key, const int& otp = 0);
};
}  // namespace kronos::base::managers

#endif  // KRONOS_BASE_MANAGERS_VARMANAGER_HPP_
