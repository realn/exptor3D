#pragma once

#include <glm/vec2.hpp>

#include "GUIRenderContext.h"

namespace core {
	class FontInfo;
}

namespace gui {
	class RenderContext;
	class TextPrinter;
	class MenuItem;

	class Menu {
	public:
		using menuitemptr_t = std::shared_ptr<MenuItem>;

		Menu(const std::string& id);
		~Menu();

		void	update(const float fTD, const core::FontInfo& fontInfo);
		RenderContext	makeRender(gui::TextPrinter& printer) const;

		void	eventMouseMove(const glm::vec2& pos);
		void	eventMoveUp();
		void	eventMoveDown();
		bool	eventEnter(std::string& outScript);

		void	addMenuItem(menuitemptr_t item);

		void	setTitle(const std::string& value);
		void	setTitlePos(const glm::vec2& value);
		void	setSize(const glm::vec2& value);
		void	setVisible(bool value, bool animate);

		const std::string& getId() const;
		bool	isVisible() const;
		bool	isAnimating() const;

	private:
		using menuitems_t = std::vector<menuitemptr_t>;

		const std::string	id;
		std::string	title;

		menuitems_t items;
		glm::vec2	titlePos;
		glm::vec2	size;

		menuitemptr_t selectedItem;

		bool	visible = true;
		float	scroll = 0.0f;
	};
}