#pragma once

#include <vector>
#include <memory>

#include "gui_Element.h"

namespace gui {
  enum class Orientation {
    Horizontal = 0,
    Vertical = 1
  };

  class ContainerElement : public gui::Element{
  public:
    using elementptr_t = std::shared_ptr<Element>;

    ~ContainerElement() override;
    
    void addElement(elementptr_t element);
    void clear();

    void setItemPadding(const glm::vec2& value);
    void setItemSpace(float value);

  protected:
    using elements_t = std::vector<elementptr_t>;

    elements_t elements;
    glm::vec2 padding;
    float itemSpace = 0.0f;
  };
}