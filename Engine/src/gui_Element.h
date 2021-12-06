#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace gui {
	enum class HAlign {
		Left,
		Center,
		Right,
	};

	enum class VAlign {
		Top,
		Middle,
		Bottom,
	};

	class RenderContext;
	class TextPrinter;

	class Element {
	protected:
		HAlign	alignH = HAlign::Center;
		VAlign	alignV = VAlign::Middle;
		glm::vec2	margin = glm::vec2(0.0f);
		glm::vec2	scale = glm::vec2(1.0f);
		glm::vec4	color = glm::vec4(1.0f);

	protected:
		Element();

	public:
		virtual ~Element();

		virtual void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) = 0;
		virtual void	update(const float timeDelta) = 0;

		void	setAlign(const HAlign hor, const VAlign ver);
		void	setMargin(const glm::vec2& value);
		void	setScale(const glm::vec2& value);
		void	setColor(const glm::vec4& value);

		HAlign	getAlignH() const;
		VAlign	getAlignV() const;
		glm::vec2	getMargin() const;
		glm::vec2	getScale() const;
		virtual glm::vec2	getSize() const = 0;

	protected:
		const glm::vec2	createPos(const glm::vec2& size, const glm::vec2& screenSize) const;
	};

}
