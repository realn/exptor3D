
#include "GUITextPrinter.h"
#include "GUIRenderContext.h"
#include "core_FontInfo.h"
#include "gui_MenuItem.h"

namespace gui {
	MenuItem::MenuItem(const std::string& id) :
		id(id),
		highlight(false),
		scrollStep(20.0f, 0.0f),
		color(1.0f) {
	}

	void MenuItem::update(const float timeDelta, const core::FontInfo& info) {
		if (size == glm::vec2(0.0f))
			size = info.getTextSize(title);

		if (!highlight) {
			if (color > 0.5f)
				color -= 1.0f * timeDelta;
			else
				color = 0.5f;

			if (scroll.x > 0.0f)
				scroll.x -= 1.0f * timeDelta;
			else
				scroll.x = 0.0f;
		}
		else {
			if (color < 1.0f)
				color += 2.0f * timeDelta;
			else
				color = 1.0f;

			if (scroll.x < 1.0f)
				scroll.x += 2.0f * timeDelta;
			else
				scroll.x = 1.0f;
		}
	}

	void MenuItem::render(gui::RenderContext& ctx, gui::TextPrinter& printer) const {
		ctx.setColor({ color, color, color, 1.0f });
		printer.print(ctx, pos + scroll * scrollStep, title);
	}

	void	MenuItem::setTitle(const std::string& value) {
		title = value;
		size = glm::vec2(0.0f);
	}

	void	MenuItem::setPos(const glm::vec2& value) {
		pos = value;
	}

	void	MenuItem::setSize(const glm::vec2& value) {
		size = value;
	}

	void	MenuItem::setHighlight(bool value) {
		highlight = value;
	}

	void	MenuItem::setScript(const std::string& value) {
		script = value;
	}

	const std::string	MenuItem::getScript() const {
		return script;
	}

	const bool	MenuItem::contains(const glm::vec2& point) const {
		return point.x > pos.x && point.x < pos.x + size.x && point.y > pos.y && point.y < pos.y + size.y;
	}
}
