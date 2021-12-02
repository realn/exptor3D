#pragma once

#include <array>

#include <CBSDL/Consts.h>
#include <CBSDL/Events.h>

namespace event {
  class Manager;
  class InputMapper {
  public:
    InputMapper();

    void addMouseMapping(cb::sdl::button button, cb::string name);
    void addKeyMapping(cb::sdl::ScanCode code, cb::string name);

    void executeEvent(Manager& manager, const cb::sdl::Event& event);

  private:
    using mousebuttons_t = std::array<cb::sdl::KeyState, 5>;
    using keys_t = std::array<cb::sdl::KeyState, static_cast<size_t>(cb::sdl::ScanCode::SDL_NUM_SCANCODES)>;
    using mousebuttonevents_t = std::map<cb::sdl::button, cb::strset>;
    using keyevents_t = std::map<cb::sdl::ScanCode, cb::strset>;

    void processMouseButton(Manager& manager, const cb::sdl::MouseButtonEvent& event);
    void processKey(Manager& manager, const cb::sdl::KeyboardEvent& event);

    mousebuttons_t mouseButtons;
    keys_t keys;

    mousebuttonevents_t mouseButtonEvents;
    keyevents_t keyEvents;
  };
}