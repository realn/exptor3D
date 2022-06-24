#pragma once

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Events.h>

#include <core_object.h>

#include <gfx_Texture.h>
#include <gfx_TextureRepository.h>
#include <gfx_RenderSystem.h>

#include <logic_Scene.h>

#include <event_Manager.h>
#include <event_InputMapper.h>

#include <gui_Main.h>

#include "ModelManager.h"

enum class GAME_STATE
{
	MAINMENU,
	LEVEL,
};

enum class MOUSE_MODE
{
	MENU,
	GAME,
};

namespace e3dt {
	class Application : public core::Object {
	public:
		Application();
		~Application() override;

		int		exec();

		float getAspectRatio() const;

	private:
		bool init();
		bool initBase();
		bool initCore();
		bool initGraphics();
		bool initGame();

		void mainLoop();

		void processAppEvents();
		bool processEvent(cb::sdl::Event& event);
		bool processWindowEvent(cb::sdl::WindowEvent& event);

		void updateTimeStep(float timeDelta);
		void update(float timeDelta);
		void render();

		std::unique_ptr<cb::sdl::System> system;
		std::shared_ptr<cb::sdl::Window> window;
		std::shared_ptr<cb::sdl::GLContext> glContext;

		std::shared_ptr<event::Manager> eventManager;
		std::shared_ptr<logic::ScriptParser> scriptParser;

		std::shared_ptr<gfx::TextureRepository> textureRepository;
		std::shared_ptr<gfx::ModelManager> modelManager;
		//CControllerList	ControllerList;
		event::InputMapper inputMapper;

		std::shared_ptr<gui::Main>	GUI;

		gfx::RenderSystem renderSystem;
		logic::Scene scene;

		float frameTime = 0.0f;
		bool run = true;
		bool active = true;
	};
}