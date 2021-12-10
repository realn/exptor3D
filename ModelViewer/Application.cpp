
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CBCore/StringConvert.h>
#include <CBSDL/Events.h>

#include <CBGL/Rendering.h>
#include <CBGL/System.h>
#include <CBGL/COpenGL.h>

#include <gui_MenuItem.h>

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
  }

  int Application::exec() {
    if (!init())
      return -1;

    mainLoop();

    return 0;
  }

  bool Application::init() {
    if (!initBase())
      return false;
    if (!initCore())
      return false;

    scriptParser->addFunc(L"loadModel", [this](const core::StrArgList& args) {
      auto filename = args.getArgUtf8(0);
      if (filename.empty())
        return;


      model = std::make_shared<gfx::Model>();
      model->load(*texRepo, "Data/Models/" + filename);
                          });

    modelMenu = std::make_shared<gui::Menu>("modelMenu", textPrinter->getFontInfo());
    modelMenu->setSize({ 1200, 800 });
    mainMenu = std::make_shared<gui::MenuMain>(getAspectRatio(window->getSize()), scriptParser);
    mainMenu->addMenu(modelMenu);

    {
      auto modelPath = std::filesystem::path(L"Data/Models");
      auto it = std::filesystem::directory_iterator{ modelPath };
      for (const auto& entry : it) {
        auto item = std::make_shared<gui::MenuItem>("", textPrinter->getFontInfo());
        auto filename = entry.path().filename().u8string();
        item->setTitle(filename);
        item->setScript("loadModel(\"" + filename + "\")");
        modelMenu->addMenuItem(item);
      }
    }

    mainMenu->push("modelMenu");
    events->addObserver(mainMenu);

    input.addKeyMapping(cb::sdl::ScanCode::UP, L"gui_move_up");
    input.addKeyMapping(cb::sdl::ScanCode::DOWN, L"gui_move_down");
    input.addKeyMapping(cb::sdl::ScanCode::RETURN, L"gui_enter");
    input.addKeyMapping(cb::sdl::ScanCode::ESCAPE, L"gui_back");
    input.addMouseMotionMapping(event::InputMapper::Axis::X, L"gui_pointer_x");
    input.addMouseMotionMapping(event::InputMapper::Axis::Y, L"gui_pointer_y");
    input.addMouseMapping(cb::sdl::button::LEFT, L"gui_enter");

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

    scriptParser = std::make_shared<logic::ScriptParser>(events);

    texRepo = std::make_unique<gfx::TextureRepository>("Data/Textures/");
    textPrinter = std::make_shared<gui::TextPrinter>(texRepo->Get("Font.tga"));

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

        input.executeEvent(*events, event, window->getSize());
      }

      events->processEvents();

      update(1.0f / 60.0f);

      render();

      glContext->swapWindow(*window);
    }
  }

  void Application::update(float timeDelta) {
    mainMenu->update(timeDelta);
  }

  void Application::render() {
    cb::gl::clearColor({ 1.0f, 0.5f, 0.5f, 1.0f });

    if (model) {
      auto mat = glm::perspective(glm::radians(90.0f), getAspectRatio(window->getSize()), 0.1f, 100.0f);

      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf(glm::value_ptr(mat));
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
      glLoadMatrixf(glm::value_ptr(mat));

      model->render(0);
    }

    auto ctx = modelMenu->makeRender(*textPrinter);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(ctx.getProjectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ctx.render();
  }
}