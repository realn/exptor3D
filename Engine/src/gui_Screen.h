#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"
#include "gui_ValueSync.h"

namespace gui {
	class Element;

	class Screen {
	public:
		~Screen();

		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize);
		void	update(const float fTD);

		void	setMargin(const glm::vec2& value);

		void addElement(std::shared_ptr<Element> element);

		const bool	load(const std::string& filename, const core::FontInfo& fontInfo);

		ValueSyncList& getValues() { return values; }

	private:
		using elementptr_t = std::shared_ptr<Element>;
		using elements_t = std::vector<elementptr_t>;

		elements_t elements;
		glm::vec2	margin;
		ValueSyncList values;
	};
}
