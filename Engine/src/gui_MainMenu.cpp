#include <fstream>

#include <CBCore/StringConvert.h>
#include <CBGL/COpenGL.h>

#include "core_FileParser.h"
#include "StrEx.h"
#include "Log.h"

#include "logic_ScriptParser.h"

#include "gui_Menu.h"
#include "gui_MenuItem.h"
#include "gui_MainMenu.h"

namespace gui {
	MenuMain::MenuMain(const float aspectRatio, std::shared_ptr<logic::ScriptParser> scriptParser) :
		scriptParser(scriptParser),
		aspectRatio(aspectRatio),
		file("-") {

		scriptParser->addFunc(L"pushMenu", [this](const core::StrArgList& args) { push(args.getArgUtf8(0)); });
		scriptParser->addFunc(L"popMenu", [this](const core::StrArgList& args) { pop(); });

		mapper.addAction(L"gui_move_up", [this](const event::EventAction&) { eventMoveUp(); });
		mapper.addAction(L"gui_move_down", [this](const event::EventAction&) { eventMoveDown(); });
		mapper.addAction(L"gui_enter", [this](const event::EventAction&) { eventEnter(); });
		mapper.addAction(L"gui_exit", [this](const event::EventAction&) { eventExit(); });
		mapper.addRange(L"gui_pointer_x", [this](const event::EventRange& r) { eventPointerMoveX(r.getValue()); });
		mapper.addRange(L"gui_pointer_y", [this](const event::EventRange& r) { eventPointerMoveY(r.getValue()); });
	}

	MenuMain::~MenuMain() = default;

	void MenuMain::addMenu(menuptr_t menu) {
		menus.push_back(menu);
	}

	MenuMain::menuptr_t MenuMain::getCurrent() const {
		if (menuStack.empty())
			return menuptr_t();

		auto index = glm::clamp(menuIndexCurrent, size_t(0), menuStack.size() - 1);
		return menuStack[index];
	}

	void MenuMain::update(float timeDelta) {
		if (menuPopped) {
			if (!menuPopped->isAnimating() && menuPopped->isVisible())
				menuPopped->setVisible(false, true);
			else if (!menuPopped->isVisible() && !menuPopped->isAnimating()) {
				menuPopped = nullptr;
			}
			else {
				menuPopped->update(timeDelta);
			}
			return;
		}

		if (menuStack.empty()) {
			menuIndexCurrent = 0;
			return;
		}

		menuIndexCurrent = glm::clamp(menuIndexCurrent, size_t(0), menuStack.size() - 1);

		auto menuCurrent = getCurrent();
		if (menuIndexCurrent == menuStack.size() - 1) {
			if (!menuCurrent->isAnimating() && !menuCurrent->isVisible())
				menuCurrent->setVisible(true, true);

			getCurrent()->update(timeDelta);
			return;
		}

		if (!menuCurrent->isAnimating() && menuCurrent->isVisible())
			menuCurrent->setVisible(false, true);

		if (!menuCurrent->isAnimating() && !menuCurrent->isVisible()) {
			menuIndexCurrent++;
			menuCurrent = getCurrent();
			menuCurrent->setVisible(true, true);
		}

		getCurrent()->update(timeDelta);
	}

	RenderContext MenuMain::makeRender(TextPrinter& printer) const {
		auto menu = getCurrent();
		if (menuPopped)
			menu = menuPopped;
		if (!menu)
			return RenderContext();

		return menu->makeRender(printer);
	}

	void	MenuMain::push(const std::string& id) {
		auto menu = findMenu(id);
		if (menu == nullptr)
			return;

		menuStack.push_back(menu);
	}

	void	MenuMain::pop() {
		if (menuStack.empty())
			return;

		menuPopped = menuStack.back();
		menuStack.pop_back();
	}

	const bool	MenuMain::isMenuAnimating() const {
		if (menuStack.empty())
			return false;

		return menuStack.back()->isAnimating();
	}

	void MenuMain::processEvent(const event::Event& event) {
		mapper.executeEvent(event);
	}

	MenuMain::menuptr_t MenuMain::findMenu(const std::string& id) {
		auto it = std::find_if(menus.begin(), menus.end(), [&](menuptr_t menu) { return menu->getId() == id; });
		if (it != menus.end())
			return *it;
		return menuptr_t();
	}

	void MenuMain::eventPointerMoveX(float posX) {
		pointerPos.x = posX;
		eventMouseMove(pointerPos);
	}

	void MenuMain::eventPointerMoveY(float posY) {
		pointerPos.y = posY;
		eventMouseMove(pointerPos);
	}

	void	MenuMain::eventMouseMove(const glm::vec2& pos) {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		menu->eventMouseMove(pos);
	}

	void	MenuMain::eventEnter() {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		std::string script;
		if (menu->eventEnter(script)) {
			scriptParser->execute(cb::fromUtf8(script));
		}
	}

	void MenuMain::eventExit() {
		pop();
	}

	void	MenuMain::eventMoveDown() {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		menu->eventMoveDown();
	}

	void	MenuMain::eventMoveUp() {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		menu->eventMoveUp();
	}

	void MenuMain::free() {
		menus.clear();
		menuStack.clear();
	}

	bool MenuMain::load(const std::string& filename, const core::FontInfo& fontInfo) {
		if (filename.empty()) {
			Log.Error("MAINMENU( " + file + " ): Pusty ci¹g znaków w œcie¿ce do pliku");
			return false;
		}

		free();

		core::FileParser parser(cb::fromUtf8(filename));

		file = filename;
		menuptr_t menu;
		std::shared_ptr<MenuItem> menuItem;
		auto itemPos = glm::vec2();
		auto itemStep = glm::vec2();
		while (parser.readLine()) {

			auto cmd = parser.getCmd();

			if (cmd == L"MENU") {
				menuItem = nullptr;
				menu = std::make_shared<Menu>(parser.getArgUtf8(0), fontInfo);
				menus.push_back(menu);
			}
			else if (cmd == L"MENUTITLE" && menu) {
				menu->setTitle(parser.getArgUtf8(0));
			}
			else if (cmd == L"MENUSIZE" && menu) {
				float height = parser.getFloat(0);
				float width = aspectRatio * height;
				menu->setSize({ width, height });
			}
			else if (cmd == L"MENUITEM" && menu) {
				menuItem = std::make_shared<MenuItem>(parser.getArgUtf8(0), fontInfo);
				menu->addMenuItem(menuItem);
			}
			else if (cmd == L"MENUITEMTITLE" && menuItem) {
				menuItem->setTitle(parser.getArgUtf8(0));
			}
			else if (cmd == L"MENUITEMACTION" && menuItem) {
				menuItem->setScript(parser.getArgUtf8(0));
			}
		}

		return true;
	}
}