#include <CBSDL/Events.h>

#include "Application.h"

namespace core {
  Application::Application() : Object(L"Application") {
    system = std::make_unique<cb::sdl::System>(cb::sdl::SystemFlag::VIDEO);
  }

  int Application::exec() {
    if (!init())
      return -1;

    mainLoop();

    return 0;
  }

  bool Application::init() {
    cb::sdl::GLAttributeMapT attribs = {
      {cb::sdl::GLAttribute::BUFFER_SIZE, 32},
      {cb::sdl::GLAttribute::DEPTH_SIZE, 24},
      {cb::sdl::GLAttribute::STENCIL_SIZE, 8},
      {cb::sdl::GLAttribute::DOUBLEBUFFER, 1}
    };

    cb::sdl::setContextCreationAttributes(attribs);

    window = std::make_unique<cb::sdl::Window>(L"ModelViewer", cb::sdl::Window::PosCentered, glm::uvec2{ 1200, 800 }, cb::sdl::WindowFlag::OPENGL);
    window->show();

    glContext = std::make_unique<cb::sdl::GLContext>(*window);

    return true;
  }

  void Application::mainLoop() {

    while (run) {
      cb::sdl::Event event;

      while (cb::sdl::Event::poll(event)) {
        if (event.getType() == cb::sdl::EventType::WINDOWEVENT) {
          auto win = event.window();
          if (win.getType() == cb::sdl::WindowEventType::CLOSE) {
            run = false;
          }
        }
      }


    }

  }
}