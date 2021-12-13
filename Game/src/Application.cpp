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

#include "Application.h"
#include "GamePlayerController.h"

#include "GUI.h"
#include "GamePlayer.h"
#include "Level.h"

namespace e3dt {
	Application::Application() 
		: core::Object(L"Application")
	{
		eventManager = std::make_shared<event::Manager>();

		scriptParser = std::make_shared<logic::ScriptParser>(eventManager);
		scriptParser->addFunc(L"quit", [this](const core::StrArgList&) { run = false; });;

		eventManager->addObserver(scriptParser);

		system = std::make_unique<cb::sdl::System>(cb::sdl::SystemFlag::VIDEO);

		RegScript();
	}

	Application::~Application() {
		report(L"Koniec pracy Aplikacji");
	}

	int	Application::exec() {
		std::srand(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

		// Czytamy plik ini, by odpowiednio wszystko ustawiæ
		core::IniFile ini(L"config.ini");
		int WindowWidth = ini.getValue<int>(L"GRAPHIC", L"WIDTH", 640);
		int WindowHeight = ini.getValue<int>(L"GRAPHIC", L"HEIGHT", 480);
		bool fullscreen = ini.getValue<bool>(L"GRAPHIC", L"FULLSCREEN", false);

		float aspectRatio = (float)WindowWidth / (float)WindowHeight;

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
			return 0;
		}

		glContext = std::make_shared<cb::sdl::GLContext>(*window);
		if (glContext == nullptr) {
			fatal(L"Nie uda³o siê stworzyæ kontekstu OpenGL!");
			return 0;									// WyjdŸ je¿eli nie zosta³o stworzone
		}

		window->show();

		cb::gl::initextensions();

		gfx::TextureRepository texManager("Data/Textures/");
		gfx::ModelManager modelManager("Data/Models/", texManager);
		CLevel level(texManager, modelManager);
		GUI = std::make_shared<CGUIMain>(texManager, scriptParser, aspectRatio, WindowHeight);

		pGLevel = &level;
		//CLocalPlayerController* pController = new CLocalPlayerController( level.GetPlayer() );
		//ControllerList.AddController( pController );

		//eventManager->AddObserver( pController );
		eventManager->addObserver(GUI);

		inputMapper.addKeyMapping(cb::sdl::ScanCode::UP, L"gui_move_up");
		inputMapper.addKeyMapping(cb::sdl::ScanCode::DOWN, L"gui_move_down");
		inputMapper.addKeyMapping(cb::sdl::ScanCode::RETURN, L"gui_enter");
		inputMapper.addKeyMapping(cb::sdl::ScanCode::ESCAPE, L"gui_back");
		inputMapper.addMouseMotionMapping(event::InputMapper::Axis::X, L"gui_pointer_x");
		inputMapper.addMouseMotionMapping(event::InputMapper::Axis::Y, L"gui_pointer_y");
		inputMapper.addMouseMapping(cb::sdl::button::LEFT, L"gui_enter");

		log(L"Inicjalizacja OpenGL");

		InitGraphics(texManager);

		GUI->ShowMenu(L"MainMenu");
		GUI->SetMode(GUI_MODE::MENU);

		MainLoop();
		return 0;
	}

	bool Application::ProcessWindowEvent(cb::sdl::WindowEvent& event) {
		switch (event.getType()) {
		case cb::sdl::WindowEventType::FOCUS_GAINED:
			active = true;
			return true;

		case cb::sdl::WindowEventType::FOCUS_LOST:
			active = false;
			return true;

		case cb::sdl::WindowEventType::CLOSE:
			runLoop = false;
			return true;

		case cb::sdl::WindowEventType::SIZE_CHANGED:
			return true;

		default:
			return false;
		}
	}

	bool Application::ProcessTextInput(cb::sdl::TextInputEvent& event) {

		//TODO CONSOLE EDITING
		return false;
	}

	const bool	Application::ProcessEvent(cb::sdl::Event& event) {
		switch (event.getType())									// SprawdŸ komunikaty
		{
		case cb::sdl::EventType::WINDOWEVENT:
			return ProcessWindowEvent(event.window());

		case cb::sdl::EventType::TEXTINPUT:
			return ProcessTextInput(event.text());

		case cb::sdl::EventType::QUIT:
			runLoop = false;
			return true;
		}

		inputMapper.executeEvent(*eventManager, event, window->getSize());
		return false;
	}

	void	Application::UpdateMouse() {
		if (MouseMode != MOUSE_MODE::GAME)
			return;

		unsigned halfScreenX = window->getSize().x / 2;
		unsigned halfScreenY = window->getSize().y / 2;

		glm::ivec2 mouseRel;
		cb::sdl::getRelativeMouseState(mouseRel);

		int diffX = mouseRel.x;
		int diffY = mouseRel.y;

		if (diffX == 0 && diffY == 0)
			return;

		//CEventMouse MouseEvent( EVENT_INPUT_TYPE::MOUSEMOVEDIF, diffX, diffY );
		//EventManager.AddEvent( *((CEvent*)&MouseEvent) );

		GUI->getScreen().getValues().onVarChanged(L"debugValue", cb::toStr(diffX));
		window->warpMouse({ (int)halfScreenX, (int)halfScreenY });
		cb::sdl::getRelativeMouseState();
	}

	void	Application::InitGraphics(gfx::TextureRepository& texManager) {
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

	}

	void	Application::MainLoop() {
		const float	TIME_STEP = 0.005f;

		float	frameTime = 0.0f;
		cb::sdl::PerformanceTimer timer;


		while (run)									// Pêtla g³ówna (dopuki done nie jest true)
		{
			frameTime += timer.getTimeDelta();

			cb::sdl::Event event;
			for (unsigned i = 0; i < 20 && cb::sdl::Event::poll(event); i++)	// Czy otrzymano komunikat?
			{
				ProcessEvent(event);
			}

			// Rysujemy scene
			if (active)								// Program jest aktywny?
			{
				GUI->UpdateCounter(timer.getTimeDelta());
				UpdateMouse();

				Render();						// Rysujemy scene

				eventManager->processEvents();

				for (unsigned i = 0; i < 100 && frameTime > TIME_STEP; i++) {
					Update(TIME_STEP);
					frameTime -= TIME_STEP;
				}

				glContext->swapWindow(*window);
			}
			//else
				//cb::sdl::Event::waitFor();

			timer.update();
		}
	}


	void	Application::Update(const float fTD) {
		GUI->Update(fTD);

		if (State != GAME_STATE::LEVEL)
			return;

		if (GUI->IsMenuAnimating())
			return;
		else {
			GUI->SetMode(GUI_MODE::SCREEN);
			MouseMode = MOUSE_MODE::GAME;
		}

		ControllerList.Update();

		pGLevel->Update(fTD);
	}

	void	Application::Render() {
		cb::gl::clearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
		cb::gl::clear(cb::gl::ClearBuffers(cb::gl::ClearBuffer::COLOR) | cb::gl::ClearBuffer::DEPTH);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (State == GAME_STATE::LEVEL && !GUI->IsMenuAnimating()) {
			{
				auto pers = glm::perspective(glm::radians(60.0f), (4.0f / 3.0f), 1.0f, 100.0f);
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(glm::value_ptr(pers));
				glMatrixMode(GL_MODELVIEW);
			}
			glLoadIdentity();

			glRotatef(pGLevel->GetPlayer().GetAngle(), 0.0f, 1.0f, 0.0f);
			glTranslatef(-pGLevel->GetPlayer().Pos.x, 0, pGLevel->GetPlayer().Pos.z);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			pGLevel->Render();

			{
				auto pers = glm::perspective(glm::radians(45.0f), (4.0f / 3.0f), 1.0f, 10.0f);
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(glm::value_ptr(pers));
				glMatrixMode(GL_MODELVIEW);

			}
			glClear(GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			pGLevel->GetPlayer().Render();
		}

		GUI->Render();
	}

	void	Application::LoadLevel(const std::string& filename) {
		if (pGLevel->LoadLevel(filename)) {
			GUI->HideMenu();
			State = GAME_STATE::LEVEL;
		}
	}

	void	Application::Print(const std::string& text) {
		GUI->PrintConsole(text);
	}
}