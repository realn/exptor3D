#pragma once

#include "gui_ContainerElement.h"

namespace gui {
  class StackElement : public ContainerElement {
  public:
    StackElement(Orientation orientation = Orientation::Vertical);
    ~StackElement() override;

    virtual void render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;
    virtual void update(const float timeDelta) override;
    virtual glm::vec2 getSize() const override;

  private:
    glm::vec2 createElementPos(size_t elementIndex, const glm::vec2& screenSize) const override;

    glm::vec2 getStepMod() const;
    glm::vec2 getConstMod() const;
    glm::vec2 maxByOrientation(const glm::vec2& value, const glm::vec2& size) const;

    Orientation orientation;
  };
}
