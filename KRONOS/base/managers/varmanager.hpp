/*
  @file base/managers/varmanager.hpp

  Stores variables to a key
*/

#ifndef _VARMANAGER_HPP_
#define _VARMANAGER_HPP_

#include <any>
#include <map>
#include <string>

namespace KRONOS {
  class VarManager {
    private:
      std::map<std::string, std::unique_ptr<std::any>> _global {};
    protected:
      template <class T>
      inline void global_set(const std::string &key, const T& value) {
        if (_global.find(key) == _global.end()) {
          _global.emplace(key, std::make_unique<std::any>(std::make_any<T>(value)));
        } else {
          *_global.at(key) = std::make_any<T>(value);
        }
      }
    public:
      template<class T>
      inline T* global_get(const std::string &key) {
        return _global.find(key) == _global.end() ? nullptr : std::any_cast<T>(_global.at(key).get());
      }
  };
}

#endif
