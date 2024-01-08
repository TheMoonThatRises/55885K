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

namespace KRONOS {
class VarManager {
 private:
    std::map<std::string, std::unique_ptr<std::any>> _global {};

 public:
    template <class T>
    inline void global_set(const std::string &key, const T& value) {
      _global.insert_or_assign(
        key,
        std::make_unique<std::any>(std::make_any<T>(value)));
    }

    template<class T>
    inline T* global_get(const std::string &key) {
      return _global.find(key) == _global.end()
        ? nullptr
        : std::any_cast<T>(_global.at(key).get());
    }
};
}  // namespace KRONOS

#endif  // KRONOS_BASE_MANAGERS_VARMANAGER_HPP_
