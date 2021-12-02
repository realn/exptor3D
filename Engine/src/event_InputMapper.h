#pragma once

#include <array>

#include <glm/vec3.hpp>

#include <CBSDL/Consts.h>
#include <CBSDL/Events.h>

namespace event {
  class Manager;
  class InputMapper {
  public:
    enum class Axis {
      X = 0, Y = 1, Z = 2
    };

    InputMapper();

    void addMouseMapping(cb::sdl::button button, cb::string name);
    void addMouseMotionMapping(Axis axis, cb::string name);
    void addKeyMapping(cb::sdl::ScanCode code, cb::string name);

    void executeEvent(Manager& manager, const cb::sdl::Event& event, glm::uvec2 windowSize);

  private:
    using mousebuttons_t = std::array<cb::sdl::KeyState, 5>;
    using keys_t = std::array<cb::sdl::KeyState, static_cast<size_t>(cb::sdl::ScanCode::SDL_NUM_SCANCODES)>;
    using mouseaxis_t = glm::vec3;
    using mousebuttonevents_t = std::map<cb::sdl::button, cb::strset>;
    using keyevents_t = std::map<cb::sdl::ScanCode, cb::strset>;
    using mouseaxisevents_t = std::map<Axis, cb::strset>;

    void processMouseButton(Manager& manager, const cb::sdl::MouseButtonEvent& event);
    void processMouseMove(Manager& manager, const cb::sdl::MouseMotionEvent& event, glm::uvec2 windowSize);
    void processMouseAxis(Manager& manager, Axis axis, cb::s32 axisValue, cb::u32 windowRange);
    void processKey(Manager& manager, const cb::sdl::KeyboardEvent& event);

    mousebuttons_t mouseButtons = { cb::sdl::KeyState::RELEASED };
    keys_t keys = { cb::sdl::KeyState::RELEASED };
    mouseaxis_t mouseAxis = { 0.0f, 0.0f, 0.0f };

    mousebuttonevents_t mouseButtonEvents;
    mouseaxisevents_t mouseAxisEvents;
    keyevents_t keyEvents;
  };
}