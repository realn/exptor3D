#pragma once

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

namespace core {
	class FontInfo;
}

namespace gui {
	class TextPrinter;
	class RenderContext;

	class MenuItem {
	public:
		MenuItem(const std::string& id);

		void	update(float timeDelta, const core::FontInfo& info);
		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer) const;

		void	setTitle(const std::string& value);
		void	setPos(const glm::vec2& value);
		void	setSize(const glm::vec2& value);
		void	setHighlight(bool value);
		void	setScript(const std::string& value);

		const std::string	getScript() const;
		const bool	contains(const glm::vec2& point) const;

	private:
		const std::string	id;
		std::string	title;
		std::string script;

		bool highlight;

		glm::vec2	pos;
		glm::vec2	size;
		glm::vec2	scroll;
		glm::vec2	scrollStep;
		float	color;
	};
}
