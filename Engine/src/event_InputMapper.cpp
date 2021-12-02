#include "event_Manager.h"
#include "event_InputMapper.h"

namespace event {
  namespace {
    float keyStateToRange(cb::sdl::KeyState value) {
      return value == cb::sdl::KeyState::PRESSED ? 1.0f : 0.0f;
    }
  }

  InputMapper::InputMapper() {
    for (auto& button : mouseButtons) {
      button = cb::sdl::KeyState::RELEASED;
    }
    for (auto& key : keys) {
      key = cb::sdl::KeyState::RELEASED;
    }
  }

  void InputMapper::addMouseMapping(cb::sdl::button button, cb::string name) {
    auto it = mouseButtonEvents.find(button);
    if (it != mouseButtonEvents.end()) {
      it->second.insert(name);
    }
    else {
      mouseButtonEvents[button] = { name };
    }
  }

  void InputMapper::addKeyMapping(cb::sdl::ScanCode code, cb::string name) {
    auto it = keyEvents.find(code);
    if (it != keyEvents.end()) {
      it->second.insert(name);
    }
    else {
      keyEvents[code] = { name };
    }
  }

  void InputMapper::executeEvent(Manager& manager, const cb::sdl::Event& event) {
    switch (event.getType()) {
    case cb::sdl::EventType::MOUSEBUTTONDOWN:
    case cb::sdl::EventType::MOUSEBUTTONUP:
      processMouseButton(manager, event.button());
      break;

    case cb::sdl::EventType::KEYDOWN:
    case cb::sdl::EventType::KEYUP:
      processKey(manager, event.key());
      break;

    default:
      break;
    }
  }

  void InputMapper::processMouseButton(Manager& manager, const cb::sdl::MouseButtonEvent& event) {
    auto it = mouseButtonEvents.find(event.getButton());
    if (it == mouseButtonEvents.end())
      return;

    auto list = it->second;
    auto id = static_cast<size_t>(event.getButton());

    auto newValue = event.getType();
    auto oldValue = mouseButtons[id];

    mouseButtons[id] = event.getType();

    if (oldValue != newValue) {
      if (oldValue == cb::sdl::KeyState::RELEASED) {
        for (auto& name : list) {
          manager.addActionEvent(name);
        }
      }

      for (auto& name : list) {
        manager.addStateEvent(name, newValue == cb::sdl::KeyState::PRESSED);
      }
    }

    for (auto& name : list) {
      manager.addRangeEvent(name, keyStateToRange(newValue), keyStateToRange(oldValue));
    }
  }

  void InputMapper::processKey(Manager& manager, const cb::sdl::KeyboardEvent& event) {
    auto it = keyEvents.find(event.getScanCode());
    if (it == keyEvents.end())
      return;

    auto list = it->second;
    auto id = static_cast<size_t>(event.getScanCode());

    auto newValue = event.getType();
    auto oldValue = keys[id];

    keys[id] = event.getType();

    if (oldValue != newValue) {
      if (oldValue == cb::sdl::KeyState::RELEASED) {
        for (auto& name : list) {
          manager.addActionEvent(name);
        }
      }

      for (auto& name : list) {
        manager.addStateEvent(name, newValue == cb::sdl::KeyState::PRESSED);
      }
    }

    for (auto& name : list) {
      manager.addRangeEvent(name, keyStateToRange(newValue), keyStateToRange(oldValue));
    }
  }
}