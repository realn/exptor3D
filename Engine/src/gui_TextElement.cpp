#include "GUIRenderContext.h"
#include "GUITextPrinter.h"
#include "gui_TextElement.h"

namespace gui {
  TextElement::TextElement(const core::FontInfo& info)
    : fontInfo(info) {
  }

  TextElement::~TextElement() = default;

  void	TextElement::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) {
    glm::vec2 pos = createPos(getSize(), screenSize);

    ctx.setColor(color);
    ctx.pushMatrix();
    ctx.scale({ scale, 1.0f });
    printer.print(ctx, pos, text);
    ctx.popMatrix();
  }

  void	TextElement::update(const float timeDelta) {

  }

  void	TextElement::setText(const std::string& value) {
    text = value;
  }

  glm::vec2	TextElement::getSize() const {
    return fontInfo.getTextSize(text) * scale;
  }

  std::string	TextElement::getText() const {
    return text;
  }

  void TextElement::setSyncValue(const std::string& value) {
    setText(value);
  }

  const std::string TextElement::getSyncValue() const {
    return getText();
  }
}