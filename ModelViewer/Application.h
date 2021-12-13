#pragma once

#include <memory>

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>

#include <event_InputMapper.h>
#include <event_Manager.h>
#include <event_Mapper.h>

#include <logic_ScriptParser.h>

#include <gfx_TextureRepository.h>
#include <gfx_Model.h>
#include <gfx_RenderSystem.h>

#include <GUITextPrinter.h>

#include <gui_MainMenu.h>
#include <gui_Menu.h>

#include <event_Observer.h>
#include <event_Mapper.h>

#include <core_object.h>

#include "ModelViewer.h"

namespace mdlview {
  class Application 
    : public core::Object
    , public event::IObserver
    , public std::enable_shared_from_this<Application>
  {
  public:
    Application();

    int exec();

    void processEvent(const event::Event& event) override;

  private:
    bool init();
    bool initBase();
    bool initCore();
    bool initMenu();
    void mainLoop();

    void processAppEvents();

    void update(float timeDelta);
    void render();

    void showMenu();

    void loadModelList();
    void loadModel(cb::string filename);
    void loadObjectList();

    bool run = true;
    event::InputMapper input;
    event::Mapper mapper;

    std::unique_ptr<cb::sdl::System> system;
    std::unique_ptr<cb::sdl::Window> window;
    std::unique_ptr<cb::sdl::GLContext> glContext;

    std::shared_ptr<event::Manager> events;
    std::shared_ptr<logic::ScriptParser> scriptParser;

    std::shared_ptr<gfx::TextureRepository> texRepo;
    std::shared_ptr<gui::TextPrinter> textPrinter;
    std::shared_ptr<gui::Menu> modelMenu;
    std::shared_ptr<gui::Menu> objectMenu;
    std::shared_ptr<gui::MenuMain> mainMenu;

    std::shared_ptr<ModelViewer> modelViewer;

    gfx::RenderSystem renderSystem;
  };
}