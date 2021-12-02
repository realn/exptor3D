#pragma once

#include <memory>

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>

#include <core_object.h>

namespace core {
  class Application : public core::Object {
  public:
    Application();

    int exec();

  private:
    bool init();
    void mainLoop();

    std::unique_ptr<cb::sdl::System> system;
    std::unique_ptr<cb::sdl::Window> window;
    std::unique_ptr<cb::sdl::GLContext> glContext;

    bool run = true;
  };
}