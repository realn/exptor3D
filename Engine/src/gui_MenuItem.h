#pragma once

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

#include "core_Transformer.h"
#include "core_FontInfo.h"

#include "gui_Element.h"

namespace gui {
	class TextPrinter;
	class RenderContext;

	class MenuItem : public Element {
	public:
		MenuItem(const std::string& id, const core::FontInfo& fontInfo);
		~MenuItem() override;

		void	update(float timeDelta) override;
		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;

		void	setTitle(const std::string& value);
		void	setHighlight(bool value);
		void	setScript(const std::string& value);

		glm::vec2 getSize() const override;

		const std::string	getScript() const;

	private:
		core::FontInfo fontInfo;
		const std::string	id;
		std::string	title;
		std::string script;

		bool highlight = false;

		core::ValueTransformer<glm::vec4> itemColor;
		core::ValueTransformer<glm::vec2> scroll;
	};
}
