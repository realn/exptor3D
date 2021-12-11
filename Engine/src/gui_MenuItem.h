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
		MenuItem(const cb::string& id, const core::FontInfo& fontInfo);
		~MenuItem() override;

		void	update(float timeDelta) override;
		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;

		void	setTitle(const cb::string& value);
		void	setHighlight(bool value);
		void	setScript(const cb::string& value);

		glm::vec2 getSize() const override;

		const cb::string	getScript() const;

	private:
		core::FontInfo fontInfo;
		const cb::string	id;
		cb::string	title;
		cb::string script;

		bool highlight = false;

		core::ValueTransformer<glm::vec4> itemColor;
		core::ValueTransformer<glm::vec2> scroll;
	};
}
