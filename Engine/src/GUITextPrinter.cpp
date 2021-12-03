#include <CBGL/COpenGL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GUIRenderContext.h"
#include "GUITextPrinter.h"

namespace gui {
	TextPrinter::TextPrinter(std::shared_ptr<gfx::Texture> texture) 
		: texture(texture)
	{
		for (int i = 0; i < 256; i++)			// Loop through all 256 lists
		{
			float cx = float(i % 16) / 16.0f;		// X position of current character
			float cy = float(i / 16) / 16.0f;		// Y position of current character

			glyphs.push_back({{cx, 1.0f - cy - 0.001f}, {0.0625f, -0.0615f}});
			//glyphs.push_back({ {0.0f, 0.0f}, {1.0f, 1.0f} });

			//glTexCoord2f(cx, 1.0f - cy - 0.0625f);		// Texture coord (Bottom left)
			//glVertex2d(0, 16);				// Vertex coord (Bottom left)
			//glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.0625f);	// Texture coord (Bottom right)
			//glVertex2i(16, 16);				// Vertex coord (Bottom right)
			//glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.001f);	// Texture coord (Top right)
			//glVertex2i(16, 0);				// Vertex coord (Top right)
			//glTexCoord2f(cx, 1.0f - cy - 0.001f);		// Texture coord (Top left)
			//glVertex2i(0, 0);				// Vertex coord (Top left)
		}
	}

	void TextPrinter::print(RenderContext& ctx, const glm::vec2& pos, const std::string& text) {
		ctx.pushTexture();

		ctx.setTexture(texture);
		auto p = pos;
		for (auto item : text) {
			auto idx = static_cast<size_t>(item);
			if (idx < 32)
				continue;
			idx -= 32;

			auto& glyph = glyphs[idx];

			ctx.addRect(p, glyphSize, glyph.tpos, glyph.tsize);
			p += advance;
		}

		ctx.popTexture();
	}

	const glm::vec2 TextPrinter::getTextSize(const std::string& text) const {
		return glm::vec2(advance.x * text.size() + glyphSize.x - advance.x, glyphSize.y - advance.y);
	}
}
