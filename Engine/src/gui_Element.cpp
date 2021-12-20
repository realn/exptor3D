#include "gui_Element.h"

namespace gui {
	Element::Element() = default;

	Element::~Element() = default;

	void Element::eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) {
	}

	void Element::eventProcess(GuiEventType type) {
	}

	void	Element::setAlign(const HAlign hor, const VAlign ver) {
		alignH = hor;
		alignV = ver;
	}

	void	Element::setMargin(const glm::vec2& value) {
		margin = value;
	}

	void	Element::setColor(const glm::vec4& value) {
		color = value;
	}

	void Element::setFocus(bool value) {
		focus = value;
	}

	HAlign Element::getAlignH() const {
		return alignH;
	}

	VAlign Element::getAlignV() const {
		return alignV;
	}

	glm::vec2	Element::getMargin() const {
		return margin;
	}

	bool Element::containsPoint(const glm::vec2& point, const glm::vec2& screenSize) const {
		auto size = getSize();
		auto pos = createPos(size, screenSize);
		return point.x > pos.x && point.x < pos.x + size.x && point.y > pos.y && point.y < pos.y + size.y;
	}

	const glm::vec2	Element::createPos(const glm::vec2& size, const glm::vec2& screenSize) const {
		glm::vec2 pos;

		switch (alignH) {
		case HAlign::Left:
			pos.x = margin.x;
			break;

		case HAlign::Center:
			pos.x = (screenSize.x - size.x) / 2.0f;
			break;

		case HAlign::Right:
			pos.x = screenSize.x - size.x - margin.x;
			break;
		}

		switch (alignV) {
		case VAlign::Top:
			pos.y = margin.y;
			break;

		case VAlign::Middle:
			pos.y = (screenSize.y - size.y) / 2.0f;
			break;

		case VAlign::Bottom:
			pos.y = screenSize.y - size.y - margin.y;
			break;
		}

		return pos;
	}
}