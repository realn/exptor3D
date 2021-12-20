
#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

#include "core_FontInfo.h"
#include "gui_MenuItem.h"

namespace gui {
	MenuItem::MenuItem(const cb::string& id, const core::FontInfo& fontInfo) 
		: ButtonElement(fontInfo)
		, id(id) 
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
		itemColor.setReverse(!focus);
		scroll.setReverse(!focus);

		itemColor.update(timeDelta);
		scroll.update(timeDelta);
	}

	void MenuItem::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
		auto pos = createPos(getSize(), screenSize);

		ctx.pushColor();
		ctx.setColor(itemColor.getValue());
		printer.print(ctx, pos + scroll.getValue(), text);
		ctx.popColor();
	}


	void	MenuItem::setScript(const cb::string& value) {
		script = value;
	}

	const cb::string	MenuItem::getScript() const {
		return script;
	}
}
