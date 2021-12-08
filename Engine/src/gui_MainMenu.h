#pragma once

#include <vector>
#include <memory>

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

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

	class MenuMain : public event::IObserver {
	public:
		using menuptr_t = std::shared_ptr<Menu>;

		MenuMain(const float aspectRatio);
		~MenuMain();

		void addMenu(menuptr_t menu);


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

		std::string file;
		float	aspectRatio = 0.0f;
		menus_t menus;
		menus_t menuStack;
		menuptr_t menuCurrent;
		glm::vec2 pointerPos;
		event::Mapper mapper;
	};
}
