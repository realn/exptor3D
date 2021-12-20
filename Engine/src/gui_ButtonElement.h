#pragma once

#include <functional>
#include "gui_TextElement.h"

namespace gui {
  class ButtonElement : public TextElement {
  public:
    using func_t = std::function<void()>;

    ButtonElement(const core::FontInfo& info);
    ~ButtonElement() override;

    virtual void eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) override;
    virtual void eventProcess(GuiEventType type) override;

    void setOnClick(func_t func);

  private:
    func_t clickFunction;
  };

}
