#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

namespace gfx {
	class Texture;
}

namespace gui {
	class RenderContext;
	class TextPrinter {
	public:
		TextPrinter(std::shared_ptr<gfx::Texture> texture);

		void	print(RenderContext& ctx, const glm::vec2& pos, const std::string& text);

		const glm::vec2	getTextSize(const std::string& text) const;

	private:
		struct glyph {
			glm::vec2 tpos;
			glm::vec2 tsize;
		};
		using glyphs_t = std::vector<glyph>;

		std::shared_ptr<gfx::Texture> texture;
		glm::vec2 glyphSize = glm::vec2(16.0f);
		glm::vec2 advance = glm::vec2(14.0f, 0.0f);
		glyphs_t glyphs;
	};
}