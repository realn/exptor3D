#include "GUIRenderContext.h"
#include "GUITextPrinter.h"
#include "gui_TextElement.h"

namespace gui {
  TextElement::TextElement(const core::FontInfo& info)
    : fontInfo(info) {
  }

  TextElement::~TextElement() = default;

  void	TextElement::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
    glm::vec2 pos = createPos(getSize(), screenSize);

    ctx.setColor(color);
    ctx.pushMatrix();
    ctx.scale({ scale, 1.0f });
    printer.print(ctx, pos, text);
    ctx.popMatrix();
  }

  void	TextElement::update(const float timeDelta) {

  }

  void	TextElement::setText(const cb::string& value) {
    text = value;
  }

  void	TextElement::setScale(const glm::vec2& value) {
    scale = value;
  }

  glm::vec2	TextElement::getSize() const {
    return fontInfo.getTextSize(text) * scale;
  }

  cb::string	TextElement::getText() const {
    return text;
  }

  glm::vec2	TextElement::getScale() const {
    return scale;
  }

  void TextElement::setSyncValue(const cb::string& value) {
    setText(value);
  }

  const cb::string TextElement::getSyncValue() const {
    return getText();
  }
}