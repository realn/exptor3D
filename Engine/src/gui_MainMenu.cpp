#include <fstream>

#include <CBCore/StringConvert.h>
#include <CBGL/COpenGL.h>

#include "core_FileParser.h"
#include "StrEx.h"

#include "logic_ScriptParser.h"

#include "gui_Menu.h"
#include "gui_MenuItem.h"
#include "gui_MainMenu.h"

namespace gui {
	MenuMain::MenuMain(const float aspectRatio, std::shared_ptr<logic::ScriptParser> scriptParser) 
		: core::Object(L"MenuMain")
		, scriptParser(scriptParser)
		, aspectRatio(aspectRatio)
	{
		log(L"initalizing menu system.");
		log(L"adding script functions.");
		scriptParser->addFunc(L"pushMenu", [this](const core::StrArgList& args) { push(args.getArg(0)); });
		scriptParser->addFunc(L"popMenu", [this](const core::StrArgList& args) { pop(); });
		report(L"functions added.");
		report(L"menu system initialized.");
	}

	MenuMain::~MenuMain() = default;

	cb::string MenuMain::getLogName() const {
		return getClassName() + L"(" + loadedFile + L"):";
	}

	void MenuMain::addMenu(menuptr_t menu) {
		menus.push_back(menu);
		menu->setEnterFunction([this](Menu::menuitemptr_t item) { invokeScript(item->getScript()); });
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

	void MenuMain::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
		auto menu = getCurrent();
		if (menuPopped)
			menu = menuPopped;
		if (!menu)
			return;

		menu->render(ctx, printer, screenSize);
	}

	void MenuMain::eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		menu->eventMouseMove(pointerPos, screenSize);
	}

	void MenuMain::eventProcess(GuiEventType type) {
		switch (type) {
		case gui::GuiEventType::Enter: eventEnter(); break;
		case gui::GuiEventType::Back: eventExit(); break;
		case gui::GuiEventType::Up: eventMoveUp(); break;
		case gui::GuiEventType::Down: eventMoveDown(); break;
		}
	}

	void	MenuMain::push(const cb::string& id) {
		auto menu = findMenu(id);
		if (menu == nullptr)
			return;

		log(L"pushed menu " + id);
		if (menuStack.empty())
			menu->setVisible(false, false);
		menuStack.push_back(menu);
	}

	void	MenuMain::pop(bool all) {
		if (menuStack.empty())
			return;

		menuPopped = menuStack.back();
		log(L"popped menu " + menuPopped->getId());
		menuStack.pop_back();
		if (all)
			menuStack.clear();
	}

	bool	MenuMain::isMenuAnimating() const {
		if (menuStack.empty())
			return false;

		return menuStack.back()->isAnimating();
	}

	bool MenuMain::isMenuVisible() const {
		if (menuStack.empty() && !menuPopped)
			return false;
		return true;
	}

	MenuMain::menuptr_t MenuMain::findMenu(const cb::string& id) {
		auto it = std::find_if(menus.begin(), menus.end(), [&](menuptr_t menu) { return menu->getId() == id; });
		if (it != menus.end())
			return *it;
		return menuptr_t();
	}

	void	MenuMain::eventEnter() {
		if (menuStack.empty())
			return;

		auto menu = getCurrent();
		if (menu->isAnimating())
			return;

		menu->eventEnter();
	}

	void MenuMain::eventExit() {
		pop();
	}

	void MenuMain::invokeScript(cb::string script) {
		scriptParser->execute(script);
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

	bool MenuMain::load(const cb::string& filename, const core::FontInfo& fontInfo) {
		log(L"loading menu file " + filename);
		core::FileParser parser(filename);
		if (!parser) {
			error(L"failed to parse file " + filename);
			return false;
		}

		loadedFile = filename;
		menuptr_t menu;
		std::shared_ptr<MenuItem> menuItem;
		auto itemPos = glm::vec2();
		auto itemStep = glm::vec2();
		while (parser.readLine()) {

			auto cmd = parser.getCmd();

			if (cmd == L"MENU") {
				menuItem = nullptr;
				menu = std::make_shared<Menu>(parser.getArg(0), fontInfo);
				addMenu(menu);
			}
			else if (cmd == L"MENUTITLE" && menu) {
				menu->setTitle(parser.getArg(0));
			}
			else if (cmd == L"MENUITEM" && menu) {
				menuItem = menu->addMenuItem(parser.getArg(0));
			}
			else if (cmd == L"MENUITEMTITLE" && menuItem) {
				menuItem->setText(parser.getArg(0));
			}
			else if (cmd == L"MENUITEMACTION" && menuItem) {
				menuItem->setScript(parser.getArg(0));
			}
		}

		writeLogLines(core::LogType::Warning, parser.getLogLines());
		report(L"menu loaded.");
		return true;
	}
}