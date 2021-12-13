#pragma once

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Events.h>

#include <core_object.h>

#include <gfx_Texture.h>
#include <gfx_TextureRepository.h>

#include <event_Manager.h>
#include <event_InputMapper.h>

#include "GameController.h"
#include "gui.h"

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
		const bool	ProcessEvent(cb::sdl::Event& event);
		bool ProcessWindowEvent(cb::sdl::WindowEvent& event);
		bool ProcessTextInput(cb::sdl::TextInputEvent& event);

	private:
		void	RegScript();
		void	InitGraphics(gfx::TextureRepository& texManager);

		void	MainLoop();

		void	UpdateMouse();
		void	Update(const float fTD);
		void	Render();

	public:
		void	LoadLevel(const std::string& filename);
		void	Print(const std::string& text);

		std::unique_ptr<cb::sdl::System> system;
		std::shared_ptr<cb::sdl::Window> window;
		std::shared_ptr<cb::sdl::GLContext> glContext;

		std::shared_ptr<event::Manager> eventManager;
		std::shared_ptr<logic::ScriptParser> scriptParser;

		CControllerList	ControllerList;
		event::InputMapper inputMapper;

		std::shared_ptr<CGUIMain>	GUI;

		GAME_STATE	State = GAME_STATE::MAINMENU;
		MOUSE_MODE	MouseMode = MOUSE_MODE::MENU;

		bool run = true;
		bool active = true;
		bool runLoop = true;

	};
}