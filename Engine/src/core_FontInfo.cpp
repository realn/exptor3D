#include "core_FontInfo.h"

namespace core {
	FontInfo::FontInfo(glm::vec2 size, glm::vec2 advance) : glyphSize(size), advance(advance) {}

	const glm::vec2& FontInfo::getGlyphSize(const char) const {
		return glyphSize;
	}

	const glm::vec2& FontInfo::getGlyphAdvance(const char) const {
		return advance;
	}

	const glm::vec2 FontInfo::getTextSize(const cb::string& text) const {
		return advance * static_cast<float>(text.size()) + glyphSize - advance;
	}

	const glm::vec2 FontInfo::getLineAdvance() const {
		return lineAdvance;
	}
}