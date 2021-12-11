#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

#include "core_FontInfo.h"

namespace gfx {
	class Texture;
}

namespace gui {
	class RenderContext;

	class TextPrinter {
	public:
		TextPrinter(std::shared_ptr<gfx::Texture> texture);

		void	print(RenderContext& ctx, const glm::vec2& pos, const cb::string& text);

		const core::FontInfo& getFontInfo() const;

	private:
		struct glyph {
			glm::vec2 tpos;
			glm::vec2 tsize;
		};
		using glyphs_t = std::vector<glyph>;

		std::shared_ptr<gfx::Texture> texture;
		glyphs_t glyphs;
		core::FontInfo fontInfo;
	};
}