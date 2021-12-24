#pragma once

#include <vector>
#include <memory>

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

#include "gui_Screen.h"
#include "core_object.h"
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
		, public logic::IFuncHolder 
		, public core::Object
		, public IScreen
	{
	public:
		using menuptr_t = std::shared_ptr<Menu>;
		using menuweakptr_t = std::weak_ptr<Menu>;

		MenuMain(const float aspectRatio, std::shared_ptr<logic::ScriptParser> scriptParser);
		~MenuMain() override;

		void update(float timeDelta) override;
		void render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;

		void eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) override;
		void eventProcess(GuiEventType type) override;

		cb::string getLogName() const override;

		void addMenu(menuptr_t menu);
		menuptr_t getCurrent() const;

		bool	load(const cb::string& filename, const core::FontInfo& fontInfo);

		void push(const cb::string& id);
		void pop(bool all = false);

		bool isMenuAnimating() const;
		bool isMenuVisible() const;

	private:
		using menus_t = std::vector<menuptr_t>;

		menuptr_t findMenu(const cb::string& id);
		void eventMoveUp();
		void eventMoveDown();
		void eventEnter();
		void eventExit();

		void invokeScript(cb::string script);

		cb::string loadedFile;
		std::shared_ptr<logic::ScriptParser> scriptParser;
		float	aspectRatio = 0.0f;
		menus_t menus;
		menus_t menuStack;
		menuptr_t menuPopped;
		size_t menuIndexCurrent = 0;
	};
}
