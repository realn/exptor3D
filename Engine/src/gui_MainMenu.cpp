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

	void MenuMain::update(float timeDelta) {
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
		pMenu->update(timeDelta);
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

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return;

		menu->eventMouseMove(pos);
	}

	void	MenuMain::eventEnter() {
		if (menuStack.empty())
			return;

		auto menu = menuStack.back();
		if (menu->isAnimating())
			return;

		std::string script;
		if (menu->eventEnter(script)) {
			//outScript.clear();
			std::string str = ClearWhiteSpace(script);
			if (str == "PopMenu()") {
				pop();
				return;
			}

			auto pos = str.find("(");
			if (pos != std::string::npos && str.substr(0, pos) == "PushMenu") {
				auto endpos = str.find(")");
				if (endpos != std::string::npos) {
					push(str.substr(pos + 1, endpos - pos - 1));
					return;
				}
			}

			//outScript = script;
			return;
		}

		return;
	}

	void MenuMain::eventExit() {
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
			//else if (cmd == L"MENUMARGIN" && menu) {
			//	itemPos = parser.getVec2FromArgs(0);
			//}
			//else if (cmd == L"MENUSTEP" && menu) {
			//	itemStep = parser.getVec2FromArgs(0);
			//}
			else if (cmd == L"MENUITEM" && menu) {
				menuItem = std::make_shared<MenuItem>(parser.getArgUtf8(0), fontInfo);
				//menuItem->setPos(itemPos);
				menu->addMenuItem(menuItem);
				//itemPos += itemStep;
			}
			else if (cmd == L"MENUITEMTITLE" && menuItem) {
				menuItem->setTitle(parser.getArgUtf8(0));
			}
			//else if (cmd == L"MENUITEMPOS" && menuItem) {
			//	menuItem->setPos(parser.getVec2FromArgs(0));
			//}
			else if (cmd == L"MENUITEMACTION" && menuItem) {
				menuItem->setScript(parser.getArgUtf8(0));
			}
		}

		return true;
	}
}