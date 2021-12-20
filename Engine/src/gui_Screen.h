#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"
#include "gui_ValueSync.h"

namespace gui {
	class Element;
	enum class GuiEventType;

	class IScreen {
	public:
		virtual ~IScreen() = default;

		virtual void render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const = 0;
		virtual void update(float timeDelta) = 0;

		virtual void eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) = 0;
		virtual void eventProcess(GuiEventType type) = 0;
	};

	class Screen : public IScreen {
	public:
		using elementptr_t = std::shared_ptr<Element>;

		~Screen() override;

		void render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;
		void update(float timeDelta) override;

		void eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) override;
		void eventProcess(GuiEventType type) override;

		void setMargin(const glm::vec2& value);

		void addElement(std::shared_ptr<Element> element);

		bool load(const cb::string& filename, const core::FontInfo& fontInfo);

		ValueSyncList& getValues() { return values; }

	private:
		using elements_t = std::vector<elementptr_t>;

		elements_t elements;
		glm::vec2	margin;
		ValueSyncList values;
	};
}
