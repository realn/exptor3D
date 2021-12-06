#pragma once

#include <glm/vec2.hpp>

#include <CBCore/Defines.h>

namespace core {
	class FontInfo {
	public:
		FontInfo() = default;
		FontInfo(glm::vec2 size, glm::vec2 advance);

		const glm::vec2& getGlyphSize(const char value) const;
		const glm::vec2& getGlyphAdvance(const char value) const;

		const glm::vec2	getTextSize(const std::string& text) const;
		const glm::vec2 getLineAdvance() const;

	private:
		glm::vec2 glyphSize = glm::vec2(16.0f);
		glm::vec2 advance = glm::vec2(14.0f, 0.0f);
		glm::vec2 lineAdvance = glm::vec2(0.0f, 16.0f);
	};

}