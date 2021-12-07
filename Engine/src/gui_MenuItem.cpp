
#include "GUITextPrinter.h"
#include "GUIRenderContext.h"
#include "core_FontInfo.h"
#include "gui_MenuItem.h"

namespace gui {
	MenuItem::MenuItem(const std::string& id, const core::FontInfo& fontInfo) : id(id), fontInfo(fontInfo) {}

	MenuItem::~MenuItem() = default;

	void MenuItem::update(const float timeDelta) {
		if (!highlight) {
			if (color.a > 0.5f)
				color.a -= 1.0f * timeDelta;
			else
				color.a = 0.5f;

			if (scroll.x > 0.0f)
				scroll.x -= 1.0f * timeDelta;
			else
				scroll.x = 0.0f;
		}
		else {
			if (color.a < 1.0f)
				color.a += 2.0f * timeDelta;
			else
				color.a = 1.0f;

			if (scroll.x < 1.0f)
				scroll.x += 2.0f * timeDelta;
			else
				scroll.x = 1.0f;
		}
	}

	void MenuItem::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
		auto pos = createPos(getSize(), screenSize);

		ctx.pushColor();
		ctx.setColor(color);
		printer.print(ctx, pos + scroll * scrollStep, title);
		ctx.popColor();
	}

	void	MenuItem::setTitle(const std::string& value) {
		title = value;
	}

	void	MenuItem::setHighlight(bool value) {
		highlight = value;
	}

	void	MenuItem::setScript(const std::string& value) {
		script = value;
	}

	glm::vec2 MenuItem::getSize() const {
		return fontInfo.getTextSize(title);
	}

	const std::string	MenuItem::getScript() const {
		return script;
	}
}
