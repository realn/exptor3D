#pragma once

#include <vector>
#include <memory>

#include "gui_Container.h"
#include "gui_Element.h"

namespace gui {
  enum class Orientation {
    Horizontal = 0,
    Vertical = 1
  };

  class ContainerElement : public Element, public Container{
  public:
    ~ContainerElement() override;
    
    void setItemPadding(const glm::vec2& value);
    void setItemSpace(float value);

    bool elementContainsPoint(elementptr_t element, const glm::vec2& point, const glm::vec2& screenSize, bool searchDown = true) const;

  protected:
    glm::vec2 createElementPos(elementptr_t element, const glm::vec2& screenSize) const;
    virtual glm::vec2 createElementPos(size_t elementIndex, const glm::vec2& screenSize) const = 0;

    glm::vec2 padding;
    float itemSpace = 0.0f;
  };
}