#include <glm/common.hpp>

#include "GUIRenderContext.h"
#include "gui_StackElement.h"

namespace gui {
  StackElement::StackElement(Orientation orientation) : orientation(orientation) {}

  StackElement::~StackElement() = default;

  void StackElement::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) {
    auto contSize = getSize();
    auto pos = createPos(contSize, screenSize);

    ctx.pushMatrix();
    ctx.translate({ pos + padding, 0.0f });
    for (auto element : elements) {
      auto elemSize = element->getSize();

      ctx.pushMatrix();
      ctx.scale({ scale, 1.0f });

      element->render(ctx, printer, elemSize);
      ctx.popMatrix();

      auto step = (elemSize + padding) * getStepMod() + getStepMod() * itemSpace;
      ctx.translate({ step, 0.0f });
    }
    ctx.popMatrix();
  }

  void StackElement::update(const float timeDelta) {
    for (auto element : elements) {
      element->update(timeDelta);
    }
  }

  glm::vec2 StackElement::getSize() const {
    auto size = glm::vec2();

    for (auto element : elements) {
      auto elemSize = element->getSize();

      size = maxByOrientation(size, elemSize);
      size += elemSize * getStepMod();
    }

    size += padding * 2.0f;
    if (!elements.empty()) {
      auto num = static_cast<float>(elements.size() - 1);
      size += getStepMod() * padding * num;
      size += getStepMod() * itemSpace * num;
    }

    return size * scale;
  }

  glm::vec2 StackElement::getStepMod() const {
    if (orientation == Orientation::Horizontal)
      return glm::vec2(1.0f, 0.0f);
    return glm::vec2(0.0f, 1.0f);
  }

  glm::vec2 StackElement::getConstMod() const {
    if (orientation == Orientation::Horizontal)
      return glm::vec2(0.0f, 1.0f);
    return glm::vec2(1.0f, 0.0f);
  }

  glm::vec2 StackElement::maxByOrientation(const glm::vec2& value, const glm::vec2& size) const {
    if (orientation == Orientation::Horizontal)
      return { value.x, glm::min(value.y, size.y) };
    return { glm::max(value.x, size.x), value.y };
  }
}