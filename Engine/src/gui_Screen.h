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
		using elementptr_t = std::shared_ptr<Element>;

		~Screen();

		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const;
		void	update(float timeDelta);

		void	setMargin(const glm::vec2& value);

		void addElement(std::shared_ptr<Element> element);

		bool elementContainsPoint(elementptr_t element, const glm::vec2& point, const glm::vec2& screenSize, bool searchDown = true) const;

		const bool	load(const std::string& filename, const core::FontInfo& fontInfo);

		ValueSyncList& getValues() { return values; }

	private:
		using elements_t = std::vector<elementptr_t>;

		elements_t elements;
		glm::vec2	margin;
		ValueSyncList values;
	};
}
