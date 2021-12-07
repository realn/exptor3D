
#include "gui_ContainerElement.h"

namespace gui {
  ContainerElement::~ContainerElement() = default;

  void ContainerElement::addElement(elementptr_t element) {
    elements.push_back(element);
  }

  void ContainerElement::clear() {
    elements.clear();
  }

  void ContainerElement::setItemPadding(const glm::vec2& value) {
    padding = value;
  }

  void ContainerElement::setItemSpace(float value) {
    itemSpace = value;
  }
}