#include <fstream>

#include <CBCore/StringConvert.h>
#include <CBGL/COpenGL.h>

#include "FileParser.h"
#include "StrEx.h"
#include "Log.h"

#include "gui_Menu.h"
#include "gui_MenuItem.h"
#include "gui_MainMenu.h"

namespace gui {
	MenuMain::MenuMain(const float aspectRatio) :
		aspectRatio(aspectRatio),
		file("-") {
	}

	MenuMain::~MenuMain() = default;

	void MenuMain::update(float timeDelta, const core::FontInfo& fontInfo) {
		if (menuStack.empty())
			return;

		auto pMenu = menuStack.back();
		if (!pMenu->isVisible() && !pMenu->isAnimating()) {
			if (menuCurrent != nullptr) {
				menuStack.push_back(menuCurrent);
				pMenu = menuCurrent;
				menuCurrent->setVisible(true, true);
				menuCurrent = nullptr;
			}
			else {
				menuStack.pop_back();
				if (!menuStack.empty()) {
					pMenu = menuStack.back();
					pMenu->setVisible(true, true);
				}
				else
					return;
			}
		}
		pMenu->update(timeDelta, fontInfo);
	}

	RenderContext MenuMain::makeRender(TextPrinter& printer) const {
		if (menuStack.empty())
			return RenderContext();

		return menuStack.back()->makeRender(printer);
	}

	void	MenuMain::push(const std::string& id) {
		auto menu = findMenu(id);
		if (menu == nullptr)
			return;

		if (!menuStack.empty()) {
			menuStack.back()->setVisible(false, true);
			menuCurrent = menu;
		}
		else {
			menu->setVisible(true, true);
			menuStack.push_back(menu);
		}
	}

	void	MenuMain::pop() {
		if (menuStack.empty())
			return;

		menuStack.back()->setVisible(false, true);
	}

	const bool	MenuMain::isMenuAnimating() const {
		if (menuStack.empty())
			return false;

		return menuStack.back()->isAnimating();
	}

	MenuMain::menuptr_t MenuMain::findMenu(const std::string& id) {
		auto it = std::find_if(menus.begin(), menus.end(), [&](menuptr_t menu) { return menu->getId() == id; });
		if (it != menus.end())
			return *it;
		return menuptr_t();
	}

	void	MenuMain::eventMouseMove(const glm::vec2& pos) {
		if (menuStack.empty())
			return;

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return;

		menu->eventMouseMove(pos);
	}

	const bool	MenuMain::eventEnter(std::string& outScript) {
		if (menuStack.empty())
			return false;

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return false;

		std::string script;
		if (menu->eventEnter(script)) {
			outScript.clear();
			std::string str = ClearWhiteSpace(script);
			if (str == "PopMenu()") {
				pop();
				return true;
			}

			auto pos = str.find("(");
			if (pos != std::string::npos && str.substr(0, pos) == "PushMenu") {
				auto endpos = str.find(")");
				if (endpos != std::string::npos) {
					push(str.substr(pos + 1, endpos - pos - 1));
					return true;
				}
			}

			outScript = script;
			return true;
		}

		return false;
	}

	void	MenuMain::eventMoveDown() {
		if (menuStack.empty())
			return;

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return;

		menu->eventMoveDown();
	}

	void	MenuMain::eventMoveUp() {
		if (menuStack.empty())
			return;

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return;

		menu->eventMoveUp();
	}

	void MenuMain::free() {
		menus.clear();
		menuStack.clear();
	}

	bool MenuMain::load(const std::string& filename) {
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
				menu = std::make_shared<Menu>(parser.getArgUtf8(0));
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
			else if (cmd == L"MENUMARGIN" && menu) {
				itemPos = parser.getVec2FromArgs(0);
			}
			else if (cmd == L"MENUSTEP" && menu) {
				itemStep = parser.getVec2FromArgs(0);
			}
			else if (cmd == L"MENUITEM" && menu) {
				menuItem = std::make_shared<MenuItem>(parser.getArgUtf8(0));
				menuItem->setPos(itemPos);
				menu->addMenuItem(menuItem);
				itemPos += itemStep;
			}
			else if (cmd == L"MENUITEMTITLE" && menuItem) {
				menuItem->setTitle(parser.getArgUtf8(0));
			}
			else if (cmd == L"MENUITEMPOS" && menuItem) {
				menuItem->setPos(parser.getVec2FromArgs(0));
			}
			else if (cmd == L"MENUITEMACTION" && menuItem) {
				menuItem->setScript(parser.getArgUtf8(0));
			}
		}

		return true;
	}
}