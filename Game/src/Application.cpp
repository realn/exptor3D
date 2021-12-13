#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CBCore/StringConvert.h>

#include <CBSDL/Cursor.h>
#include <CBSDL/Funcs.h>
#include <CBSDL/Timer.h>

#include <CBGL/Rendering.h>
#include <CBGL/State.h>
#include <CBGL/COpenGL.h>
#include <CBGL/System.h>

#include <core_IniFile.h>
#include <gui.h>
#include <gfx_Frame.h>

#include "game_Object.h"

#include "ModelManager.h"
#include "Application.h"

namespace e3dt {
  Application::Application()
    : core::Object(L"Application") {
    eventManager = std::make_shared<event::Manager>();

    scriptParser = std::make_shared<logic::ScriptParser>(eventManager);
    scriptParser->addFunc(L"quit", [this](const core::StrArgList&) { run = false; });;

    eventManager->addObserver(scriptParser);

    system = std::make_unique<cb::sdl::System>(cb::sdl::SystemFlag::VIDEO);
  }

  Application::~Application() {
    report(L"Koniec pracy Aplikacji");
  }

  int	Application::exec() {
    if (!init()) {
      fatal(L"Application initialization failed.");
      return -1;
    }

    mainLoop();
    return 0;
  }

  float Application::getAspectRatio() const {
    auto size = window->getSize();
    return static_cast<float>(size.x) / static_cast<float>(size.y);
  }

  bool Application::init() {
    if (!initBase()) {
      fatal(L"Failed to initialize basics.");
      return false;
    }

    if (!initCore()) {
      fatal(L"Failed to initialize core.");
      return false;
    }

    if (!initGraphics()) {
      fatal(L"Failed to initialize graphics.");
      return false;
    }

    if (!initGame()) {
      fatal(L"failed to initialize game.");
      return false;
    }

    return true;
  }

  bool Application::initBase() {
    std::srand(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

    // Czytamy plik ini, by odpowiednio wszystko ustawiæ
    core::IniFile ini(L"config.ini");
    int WindowWidth = ini.getValue<int>(L"GRAPHIC", L"WIDTH", 640);
    int WindowHeight = ini.getValue<int>(L"GRAPHIC", L"HEIGHT", 480);
    bool fullscreen = ini.getValue<bool>(L"GRAPHIC", L"FULLSCREEN", false);

    cb::sdl::GLAttributeMapT attribs = {
      {cb::sdl::GLAttribute::BUFFER_SIZE, 32},
      {cb::sdl::GLAttribute::DEPTH_SIZE, 24},
      {cb::sdl::GLAttribute::STENCIL_SIZE, 8},
      {cb::sdl::GLAttribute::DOUBLEBUFFER, 1}
    };

    cb::sdl::setContextCreationAttributes(attribs);

    window = std::make_shared<cb::sdl::Window>(L"Expert 3D Tournament", cb::sdl::Window::PosCentered, glm::uvec2{ WindowWidth, WindowHeight }, cb::sdl::WindowFlag::OPENGL);
    if (window == nullptr) {
      fatal(L"Nie uda³o siê stworzyæ okna!");
      return false;
    }

    glContext = std::make_shared<cb::sdl::GLContext>(*window);
    if (glContext == nullptr) {
      fatal(L"Nie uda³o siê stworzyæ kontekstu OpenGL!");
      return false;									// WyjdŸ je¿eli nie zosta³o stworzone
    }

    window->show();

    cb::gl::initextensions();
    return true;
  }

  bool Application::initCore() {
    inputMapper.addKeyMapping(cb::sdl::ScanCode::UP, L"gui_move_up");
    inputMapper.addKeyMapping(cb::sdl::ScanCode::DOWN, L"gui_move_down");
    inputMapper.addKeyMapping(cb::sdl::ScanCode::RETURN, L"gui_enter");
    inputMapper.addKeyMapping(cb::sdl::ScanCode::ESCAPE, L"gui_back");
    inputMapper.addMouseMotionMapping(event::InputMapper::Axis::X, L"gui_pointer_x");
    inputMapper.addMouseMotionMapping(event::InputMapper::Axis::Y, L"gui_pointer_y");
    inputMapper.addMouseMapping(cb::sdl::button::LEFT, L"gui_enter");

    textureRepository = std::make_shared<gfx::TextureRepository>("Data/Textures/");
    modelManager = std::make_shared<gfx::ModelManager>(L"Data/Models/", textureRepository);

    GUI = std::make_shared<CGUIMain>(*textureRepository, scriptParser, getAspectRatio(), window->getSize().y);
    eventManager->addObserver(GUI);

    GUI->ShowMenu(L"MainMenu");
    GUI->SetMode(GUI_MODE::SCREEN);

    return true;
  }

  bool Application::initGraphics() {
    log(L"Inicjalizacja OpenGL");
    {
      auto state = cb::gl::getDepthState();
      state.Func = cb::gl::DepthFunc::LEQUAL;
      cb::gl::setState(state);
    }
    {
      auto state = cb::gl::getBlendState();
      state.setFunc(cb::gl::BlendFactor::SRC_ALPHA, cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA);
      cb::gl::setState(state);
    }
    glEnable(GL_TEXTURE_2D);
    //GUI->SetLoadLevelFunc( LoadLevel );
    //GUI->SendConMsg( "=====EXPERT 3D TOURNAMENT ENGINE=====", false );
    //GUI->SendConMsg( "STEROWNIK: " + GLRender.GetRndInfo( RENDER_RENDERER ), false );
    //GUI->SendConMsg( "WERSJA: " + GLRender.GetRndInfo( RENDER_VERSION ), false );
    //GUI->SendConMsg( "INICJALIZACJA GLOWNA", false );

    //GUI->SendConMsg( "Ustawianie OpenGL...", false );
    glShadeModel(GL_SMOOTH);			    //Ustawienie £adnego cieniowania

    cb::gl::clearColor({ 0.0f, 0.0f, 0.0f, 0.5f });
    cb::gl::clearDepth(1.0f);
    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    cb::gl::setHint(cb::gl::HintTarget::LINE_SMOOTH, cb::gl::HintMode::NICEST);
    cb::gl::setHint(cb::gl::HintTarget::POINT_SMOOTH, cb::gl::HintMode::NICEST);
    cb::gl::setHint(cb::gl::HintTarget::POLYGON_SMOOTH, cb::gl::HintMode::NICEST);

    //double p1[] = { 0.0, 1.0, 0.0, -glm::vec3( 0.0f, -5.01f, 0.0f ).Dot( glm::vec3( 0.0f, 1.0f, 0.0f ) ) };
    //double p2[] = { 0.0, -1.0, 0.0, -glm::vec3( 0.0f, -5.01f, 0.0f ).Dot( glm::vec3( 0.0f, -1.0f, 0.0f ) ) };
    //glClipPlane( GL_CLIP_PLANE0, p1 );
    //glClipPlane( GL_CLIP_PLANE1, p2 );

    //glEnable( GL_CLIP_PLANE0 );
    glEnable(GL_COLOR_MATERIAL);
    //glDisable( GL_CULL_FACE );
    glDisable(GL_LIGHTING);

#ifdef LIGHT_TEST
    float lDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float lSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lSpecular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lDiffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lAmbient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lSpecular);
    glEnable(GL_LIGHTING);
#endif

    //GUI->SendConMsg( "Zakonczono", false );

    //GUI->SendConMsg( "Inicjalizacja silnika...", false );
    ////SMBlur.Init();
    ////MenuModel = modelManager.Get( "menumodel.glm" );
    ////MainPlayer.SetArmor( 100.0f );
    ////SEManager.MaxSpec = 100;

    //GUI->SendConMsg( "Zakonczono", false );
    //GUI->SendConMsg( "INICJALIZACJA GLOWNA ZAKONCZONA", false );

    return true;
  }

  bool Application::initGame() {

    auto obj = std::make_shared<game::Object>(nullptr);

    obj->setPosition({ 0.0f, 0.0f, -10.0f });
    obj->setModel(modelManager->get(L"rocketlun-model.glm"));

    scene.setRootNode(obj);

    return true;
  }

  void	Application::mainLoop() {
    cb::sdl::PerformanceTimer timer;

    while (run) {
      processAppEvents();

      if (active) {
        eventManager->processEvents();

        updateTimeStep(timer.getTimeDelta());

        render();

        glContext->swapWindow(*window);
      }

      timer.update();
    }
  }

  void Application::processAppEvents() {
    cb::sdl::Event event;
    for (unsigned i = 0; i < 20 && cb::sdl::Event::poll(event); i++) {
      processEvent(event);
    }
  }

  bool Application::processEvent(cb::sdl::Event& event) {
    switch (event.getType())									// SprawdŸ komunikaty
    {
    case cb::sdl::EventType::WINDOWEVENT:
      return processWindowEvent(event.window());

    case cb::sdl::EventType::QUIT:
      run = false;
      return true;
    }

    inputMapper.executeEvent(*eventManager, event, window->getSize());
    return false;
  }

  bool Application::processWindowEvent(cb::sdl::WindowEvent& event) {
    switch (event.getType()) {
    case cb::sdl::WindowEventType::FOCUS_GAINED:
      active = true;
      return true;

    case cb::sdl::WindowEventType::FOCUS_LOST:
      active = false;
      return true;

    case cb::sdl::WindowEventType::CLOSE:
      run = false;
      return true;

    case cb::sdl::WindowEventType::SIZE_CHANGED:
      return true;

    default:
      return false;
    }
  }

  void Application::updateTimeStep(float timeDelta) {
    const float	TIME_STEP = 0.005f;

    frameTime += timeDelta;
    for (unsigned i = 0; i < 100 && frameTime > TIME_STEP; i++) {
      update(TIME_STEP);
      frameTime -= TIME_STEP;
    }
  }

  void	Application::update(float timeDelta) {
    scene.update(timeDelta);

    GUI->Update(timeDelta);
  }

  void	Application::render() {
    cb::gl::clearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
    cb::gl::clear(cb::gl::ClearBuffers(cb::gl::ClearBuffer::COLOR) | cb::gl::ClearBuffer::DEPTH);

    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    gfx::Frame frame;
    frame.setProjectionMatrix(glm::perspective(glm::radians(60.0f), getAspectRatio(), 1.0f, 100.0f));

    scene.queueRender(frame);

    renderSystem.render(frame);

    GUI->Render();
  }
}