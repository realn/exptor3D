
#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

#include "core_FontInfo.h"
#include "gui_MenuItem.h"

namespace gui {
	MenuItem::MenuItem(const cb::string& id, const core::FontInfo& fontInfo) 
		: id(id), fontInfo(fontInfo) 
	{
		itemColor.setRange({ 0.8f, 0.8f, 0.8, 0.9f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		itemColor.setDuration(std::chrono::milliseconds(400));
		itemColor.setEnabled(true);

		scroll.setRange({ 0.0f, 0.0f }, { 10.0f, 0.0f });
		scroll.setDuration(std::chrono::milliseconds(400));
		scroll.setEnabled(true);
	}

	MenuItem::~MenuItem() = default;

	void MenuItem::update(const float timeDelta) {
		itemColor.setReverse(!highlight);
		scroll.setReverse(!highlight);

		itemColor.update(timeDelta);
		scroll.update(timeDelta);
	}

	void MenuItem::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
		auto pos = createPos(getSize(), screenSize);

		ctx.pushColor();
		ctx.setColor(itemColor.getValue());
		printer.print(ctx, pos + scroll.getValue(), title);
		ctx.popColor();
	}

	void	MenuItem::setTitle(const cb::string& value) {
		title = value;
	}

	void	MenuItem::setHighlight(bool value) {
		highlight = value;
	}

	void	MenuItem::setScript(const cb::string& value) {
		script = value;
	}

	glm::vec2 MenuItem::getSize() const {
		return fontInfo.getTextSize(title);
	}

	const cb::string	MenuItem::getScript() const {
		return script;
	}
}
