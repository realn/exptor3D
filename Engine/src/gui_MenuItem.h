#pragma once

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

#include "core_Transformer.h"
#include "core_FontInfo.h"

#include "gui_ButtonElement.h"

namespace gui {
	class TextPrinter;
	class RenderContext;

	class MenuItem : public ButtonElement {
	public:
		MenuItem(const cb::string& id, const core::FontInfo& fontInfo);
		~MenuItem() override;

		void	update(float timeDelta) override;
		void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;

		void	setScript(const cb::string& value);

		const cb::string	getScript() const;

	private:
		const cb::string	id;
		cb::string script;

		core::ValueTransformer<glm::vec4> itemColor;
		core::ValueTransformer<glm::vec2> scroll;
	};
}
