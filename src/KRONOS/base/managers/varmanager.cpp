/*
  Copyright 2025 Peter Duanmu

  @file base/managers/varmanager.cpp

  Stores variables to a key
*/

#include <memory>
#include <string>

#include "KRONOS/base/managers/varmanager.hpp"

#include "KRONOS/assets/errors.hpp"
#include "KRONOS/assets/logger.hpp"

namespace kronos::base::managers {
using assets::Logger;
using assets::InvalidElevatedVariableAccess;

template <class T>
void VarManager::Variable::update_value(const T& value) {
  _value = std::make_unique<std::any>(std::make_any<T>(value));
}

std::any* VarManager::Variable::get_value() {
  return _value.get();
}

template <class T>
VarManager::Variable::Variable(const T& value, const bool& is_elevated)
: _value(std::make_unique<std::any>(std::make_any<T>(value))),
_is_elevated(is_elevated) {}

VarManager::Variable::~Variable() {
  _value.reset(nullptr);
}

bool VarManager::var_exists(const std::string& key) {
  return _global.find(key) != _global.end();
}

bool VarManager::is_elevated(const std::string& key) {
  return _global.at(key)->_is_elevated;
}

VarManager::VarManager(const std::string& secret_key) : _htop(secret_key) {}

void VarManager::global_set(
  const std::string& key,
  const std::any& value,
  const int& otp) {
  bool is_verified = _htop.verify(otp);

  if (var_exists(key)) {
    if (is_elevated(key) && !is_verified) {
      throw new InvalidElevatedVariableAccess(key);
    }

    Logger::info("setting elevated variable '" + key + "'");

    _global.at(key)->update_value(value);
  } else {
    (void) _global.insert(std::make_pair(
      key,
      std::make_unique<Variable>(value, is_verified)));
  }
}

void VarManager::global_delete(const std::string& key, const int& otp) {
  if (is_elevated(key) && !_htop.verify(otp)) {
    throw new InvalidElevatedVariableAccess(key);
  }

  Logger::info("deleting elevated variable '" + key + "'");

  _global.at(key)->~Variable();
  _global.at(key).reset(nullptr);
  (void) _global.erase(key);
}
}  // namespace kronos::base::managers
