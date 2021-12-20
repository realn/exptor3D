#include "gui_ButtonElement.h"

namespace gui {
  ButtonElement::ButtonElement(const core::FontInfo& info) 
    : TextElement(info) 
  {}

  ButtonElement::~ButtonElement() = default;

  void ButtonElement::eventPointerMove(const glm::vec2 & pointerPos, const glm::vec2 & screenSize) {
  }

  void ButtonElement::eventProcess(GuiEventType type) {
    if (type == GuiEventType::Enter && focus) {
      clickFunction();
    }
  }

  void ButtonElement::setOnClick(func_t func) {
    clickFunction = func;
  }
}