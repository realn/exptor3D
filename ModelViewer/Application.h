#pragma once

#include <memory>

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>

#include <event_InputMapper.h>
#include <event_Manager.h>

#include <gfx_TextureRepository.h>

#include <GUITextPrinter.h>

#include <gui_MainMenu.h>
#include <gui_Menu.h>

#include <core_object.h>

namespace mdlview {
  class Application : public core::Object {
  public:
    Application();

    int exec();

  private:
    bool init();
    void mainLoop();

    void update(float timeDelta);
    void render();

    bool run = true;
    event::Manager events;
    event::InputMapper input;

    std::unique_ptr<cb::sdl::System> system;
    std::unique_ptr<cb::sdl::Window> window;
    std::unique_ptr<cb::sdl::GLContext> glContext;

    std::unique_ptr<gfx::TextureRepository> texRepo;
    std::shared_ptr<gui::TextPrinter> textPrinter;
    std::shared_ptr<gui::Menu> modelMenu;
    std::shared_ptr<gui::MenuMain> mainMenu;

  };
}