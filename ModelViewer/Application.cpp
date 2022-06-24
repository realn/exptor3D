
#include <filesystem>

#include <core_Files.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CBCore/StringConvert.h>
#include <CBSDL/Events.h>
#include <CBSDL/Timer.h>

#include <CBGL/Rendering.h>
#include <CBGL/System.h>
#include <CBGL/State.h>
#include <CBGL/COpenGL.h>

#include <gui_MenuItem.h>
#include <gfx_Frame.h>

#include "Application.h"

namespace mdlview {
  namespace {
    template<class _Type>
    float getAspectRatio(const _Type& value) {
      if (static_cast<float>(value.y) == 0.0f)
        return 1.0f;
      return static_cast<float>(value.x) / static_cast<float>(value.y);
    }
  }

  Application::Application() : Object(L"Application") {
    system = std::make_unique<cb::sdl::System>(cb::sdl::SystemFlag::VIDEO);

    mapper.addAction(L"app_showmenu", [this](const event::EventAction&) { showMenu(); });
  }

  int Application::exec() {
    if (!init())
      return -1;

    mainLoop();

    return 0;
  }

  void Application::processEvent(const event::Event& event) {
    mapper.executeEvent(event);
  }

  bool Application::init() {
    if (!initBase())
      return false;
    if (!initCore())
      return false;
    if (!initMenu())
      return false;

    modelViewer = std::make_shared<ModelViewer>(texRepo);
    events->addObserver(modelViewer);

    scriptParser->addFunc(L"loadModel", [this](const core::StrArgList& args) { loadModel(args.getArg(0)); });
    scriptParser->addFunc(L"selectObject", [this](const core::StrArgList& args) { modelViewer->selectObjName(args.getArg(0)); });

    input.addKeyMapping(cb::sdl::ScanCode::UP, L"gui_move_up");
    input.addKeyMapping(cb::sdl::ScanCode::DOWN, L"gui_move_down");
    input.addKeyMapping(cb::sdl::ScanCode::RETURN, L"gui_enter");
    input.addKeyMapping(cb::sdl::ScanCode::ESCAPE, L"gui_back");
    input.addMouseMotionMapping(event::InputMapper::Axis::X, L"gui_pointer_x");
    input.addMouseMotionMapping(event::InputMapper::Axis::Y, L"gui_pointer_y");
    input.addMouseMapping(cb::sdl::button::LEFT, L"gui_enter");

    input.addKeyMapping(cb::sdl::ScanCode::ESCAPE, L"app_showmenu");
    input.addKeyMapping(cb::sdl::ScanCode::LEFT, L"viewer_rotate_model_left");
    input.addKeyMapping(cb::sdl::ScanCode::RIGHT, L"viewer_rotate_model_right");
    input.addKeyMapping(cb::sdl::ScanCode::F5, L"viewer_reload_model");

    {
      auto state = cb::gl::getDepthState();
      state.enabled = true;
      state.Func = cb::gl::DepthFunc::LEQUAL;
      cb::gl::setState(state);
    }

    return true;
  }

  bool Application::initBase() {
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

    cb::gl::initextensions();
    return true;
  }

  bool Application::initCore() {
    events = std::make_shared<event::Manager>();
    events->addObserver(shared_from_this());

    scriptParser = std::make_shared<logic::ScriptParser>(events);

    texRepo = std::make_unique<gfx::TextureRepository>("Data/Textures/");
    textPrinter = std::make_shared<gui::TextPrinter>(texRepo->Get("Font.tga"));

    guiMain = std::make_shared<gui::Main>(texRepo, scriptParser, window->getSize());

    

    return true;
  }

  bool Application::initMenu() {
    auto size = glm::uvec2(1200, 800);
    mainMenu = std::make_shared<gui::MenuMain>(getAspectRatio(window->getSize()), scriptParser);

    auto menu = std::make_shared<gui::Menu>(L"main", textPrinter->getFontInfo());
    menu->addMenuItemPush(L"modelLoadFromFile", L"Load model from file", L"modelMenu");
    menu->addMenuItemPush(L"modelSelectObject", L"Select object from Model", L"objectMenu");

    modelMenu = std::make_shared<gui::Menu>(L"modelMenu", textPrinter->getFontInfo());

    objectMenu = std::make_shared<gui::Menu>(L"objectMenu", textPrinter->getFontInfo());

    mainMenu->addMenu(menu);
    mainMenu->addMenu(modelMenu);
    mainMenu->addMenu(objectMenu);

    mainMenu->push(L"main");

    loadModelList();



    return true;
  }

  void Application::mainLoop() {
    cb::sdl::PerformanceTimer timer;

    while (run) {
      processAppEvents();

      events->processEvents();

      update(timer.getTimeDelta());

      render();

      glContext->swapWindow(*window);

      timer.update();
    }
  }

  void Application::processAppEvents() {
    cb::sdl::Event event;
    while (cb::sdl::Event::poll(event)) {
      if (event.getType() == cb::sdl::EventType::WINDOWEVENT) {
        auto win = event.window();
        if (win.getType() == cb::sdl::WindowEventType::CLOSE) {
          run = false;
        }
      }
      input.executeEvent(*events, event, window->getSize());
    }
  }

  void Application::update(float timeDelta) {
    if (!mainMenu->isMenuVisible()) {
      modelViewer->update(timeDelta);
    }

    mainMenu->update(timeDelta);
  }

  void Application::render() {
    cb::gl::clearColor({ 1.0f, 0.5f, 0.5f, 1.0f });
    cb::gl::clear(cb::gl::ClearBuffers(cb::gl::ClearBuffer::COLOR) | cb::gl::ClearBuffer::DEPTH);

    gfx::Frame frame;
    frame.setProjectionMatrix(glm::perspective(glm::radians(110.0f), getAspectRatio(window->getSize()), 0.1f, 100.0f));
    modelViewer->render(frame);

    auto ctx = mainMenu->makeRender(*textPrinter);

    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    renderSystem.render(frame);

    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, false);

    renderSystem.render(ctx);
  }

  void Application::showMenu() {
    if (!mainMenu->isMenuVisible()) {
      mainMenu->push(L"main");
    }
    else {
      mainMenu->pop(true);
    }
  }

  void Application::loadModelList() {
    auto modelPath = std::filesystem::path(L"Data/Models");
    auto files = core::getFileList(L"Data/Models", { L".glm" });

    modelMenu->clearItems();
    for (const auto& filename : files) {
      modelMenu->addMenuItem(L"", filename, L"loadModel(\"" + filename + L"\")");
    }
    modelMenu->addMenuItemPop(L"return", L"Return");
  }

  void Application::loadModel(cb::string filename) {
    modelViewer->loadModel(filename); 
    loadObjectList();
    mainMenu->pop(true);
  }

  void Application::loadObjectList() {
    auto names = modelViewer->getObjectNames();
    objectMenu->clearItems();
    for (const auto& name : names) {
      objectMenu->addMenuItem(L"", name, L"selectObject(\"" + name + L"\")");
    }
    objectMenu->addMenuItemPop(L"return", L"Return");
  }

}