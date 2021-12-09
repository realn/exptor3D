#pragma once

#include <vector>
#include <memory>

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

#include "logic_ScriptParser.h"
#include "event_Observer.h"
#include "event_Mapper.h"

namespace core {
	class FontInfo;
}

namespace gui {
	class Menu;
	class RenderContext;
	class TextPrinter;

	enum class MENU_TAG;

	class MenuMain 
		: public std::enable_shared_from_this<MenuMain>
		, public event::IObserver
		, public logic::IFuncHolder 
	{
	public:
		using menuptr_t = std::shared_ptr<Menu>;

		MenuMain(const float aspectRatio, std::shared_ptr<logic::ScriptParser> scriptParser);
		~MenuMain() override;

		void addMenu(menuptr_t menu);
		menuptr_t getCurrent() const;

		void	update(const float timeDelta);
		RenderContext	makeRender(gui::TextPrinter& printer) const;

		bool	load(const std::string& filename, const core::FontInfo& fontInfo);
		void	free();

		void	push(const std::string& id);
		void	pop();

		const bool	isMenuAnimating() const;

		void	processEvent(const event::Event& event) override;

	private:
		using menus_t = std::vector<menuptr_t>;

		menuptr_t findMenu(const std::string& id);
		void eventPointerMoveX(float posX);
		void eventPointerMoveY(float posY);
		void eventMouseMove(const glm::vec2& pos);
		void eventMoveUp();
		void eventMoveDown();
		void eventEnter();
		void eventExit();

		std::shared_ptr<logic::ScriptParser> scriptParser;
		std::string file;
		float	aspectRatio = 0.0f;
		menus_t menus;
		menus_t menuStack;
		menuptr_t menuPopped;
		size_t menuIndexCurrent = 0;
		glm::vec2 pointerPos;
		event::Mapper mapper;
	};
}
