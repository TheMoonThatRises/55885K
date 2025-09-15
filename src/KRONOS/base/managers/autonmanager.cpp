/*
  Copyright 2025 Peter Duanmu

  @file base/managers/autonmanager.cpp

  Autonomous manager for KRONOS
*/

#include <memory>
#include <string>
#include <utility>

#include "KRONOS/base/managers/autonmanager.hpp"

#include "KRONOS/assets/asserts.hpp"

namespace kronos::base::managers {
using assets::assert_not_nullptr;
using assets::side_color;
using assets::Logger;

using internal::HOTP;

std::pair<std::string, std::function<void()>> AutonomousManager::autonByIndex(
  const size_t& index
) {
  assert(index <= _autons.size());

  return *std::next(_autons.begin(), index);
}

void AutonomousManager::button_listener(lv_event_t* e) {
  assert_not_nullptr(_controller, "KRONOS::Controller");
  assert_not_nullptr(_varManager, "KRONOS::VarManager");
  assert_not_nullptr(_hotp, "KOTP::HOTP");

  int id = reinterpret_cast<intptr_t>(lv_event_get_user_data(e));

  switch (id) {
    case S_AUTON:
      _currentAutonIndex = (_currentAutonIndex + 1) % _autons.size();

      _currentAuton = autonByIndex(_currentAutonIndex).first;
      _controller->set_text("Auton << " + _currentAuton);
      break;
    case S_COLOR:
      auto *side = _varManager->global_get<side_color>("side");

      assert(side);

      auto newColor = *side == side_color::S_BLUE
          ? side_color::S_RED
          : side_color::S_BLUE;

      _varManager->global_set("side", newColor, _hotp->get_code());

      auto color_text =
        std::string(newColor == side_color::S_BLUE ? "BLUE" : "RED");

      _controller->set_text("Color << " + color_text);
      break;
  }
}

void AutonomousManager::add(
  const std::string& name,
  std::function<void()> auton) {
  (void) _autons.insert({name, auton});
}

void AutonomousManager::set_assets(Controller* controller) {
  assert_not_nullptr(controller, "KRONOS::Controller");

  _controller = controller;
}

void AutonomousManager::run() {
  assert_not_nullptr(_controller, "KRONOS::Controller");

  if (!_currentAuton.empty() && _currentAuton != "noauton") {
    Logger::info("Running auton '" + _currentAuton + "'");
    _controller->set_text("Rng auton '" + _currentAuton + "'");

    _autons.at(_currentAuton)();
  } else {
    Logger::warn("Skipping auton...");

    _controller->set_text("Skipping auton...");
  }
}

void AutonomousManager::load_auton() {
  assert_not_nullptr(_taskManager, "KRONOS::TaskManager");
  assert_not_nullptr(_controller, "KRONOS::Controller");

  if (!_taskManager->get_task(_taskName)) {
    Logger::info("Starting auton selection");

    Logger::info("Attempting to load auton event");
    (void) _taskManager->add_task(
      _taskName,
      pros::Task([&]() {
        _currentAuton = autonByIndex(_currentAutonIndex).first;

        _controller->set_text("Auton << " + _currentAuton);

        while (true) {
          lv_obj_clean(lv_screen_active());

          lv_obj_t* title = lv_label_create(lv_screen_active());
          lv_label_set_text(title, "Auton buttons");
          lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

          lv_obj_t* autonbtn = lv_button_create(lv_screen_active());
          // Enable resizing horizontally and vertically
          lv_obj_set_size(autonbtn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_align(autonbtn, LV_ALIGN_TOP_MID, 0, 10);
          // Set a unique number for the button
          lv_obj_add_event_cb(autonbtn,
            button_listener,
            LV_EVENT_CLICKED,
            reinterpret_cast<void*>(S_AUTON));

          lv_obj_t* autonlabel = lv_label_create(autonbtn);
          lv_label_set_text(autonlabel, _currentAuton.c_str());

          lv_obj_t* colorbtn = lv_button_create(lv_screen_active());
          // Enable resizing horizontally and vertically
          lv_obj_set_size(colorbtn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_align(colorbtn, LV_ALIGN_TOP_MID, 0, 80);
          // Set a unique number for the button
          lv_obj_add_event_cb(
            colorbtn,
            button_listener,
            LV_EVENT_CLICKED,
            reinterpret_cast<void*>(S_COLOR));

          // auto current_colour =
          //   _varManager->global_get<KUtil::side_color>("side");

          // assert(current_colour);

          // auto color_str = std::string(
          //   *current_colour == KUtil::S_BLUE
          //     ? "BLUE"
          //     : "RED");
          // lv_obj_t* colorlabel = lv_label_create(colorbtn, nullptr);
          // lv_label_set_text(colorlabel, color_str.c_str());

          pros::delay(200);
        }
      },
      TASK_PRIORITY_MAX,
      TASK_STACK_DEPTH_DEFAULT,
      _taskName.c_str()));
  } else {
    Logger::warn("Auton selector already initialised");
  }
}

void AutonomousManager::unload_auton_threads() {
  Logger::info("Attempting to unload auton selector");
  (void) _taskManager->kill_task(_taskName);
  lv_obj_clean(lv_screen_active());
}

void AutonomousManager::set_auton(const std::string& name) {
  _currentAuton = name;
  _currentAutonIndex = std::distance(_autons.begin(), _autons.find(name));
}

AutonomousManager::AutonomousManager(
  VarManager *varManager,
  TaskManager *taskManager,
  HOTP *hotp) {
  assert(varManager);
  assert(taskManager);
  assert(hotp);

  _varManager = varManager;
  _taskManager = taskManager;
  _hotp = hotp;
}

AutonomousManager::~AutonomousManager() {
  unload_auton_threads();
}
}  // namespace kronos::base::managers
