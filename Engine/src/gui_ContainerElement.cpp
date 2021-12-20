
#include "gui_ContainerElement.h"

namespace gui {
  ContainerElement::~ContainerElement() = default;

  void ContainerElement::eventPointerMove(const glm::vec2 & pointerPos, const glm::vec2 & screenSize) {
    if (!containsPoint(pointerPos, screenSize)) {
      for (auto item : elements) {
        item->setFocus(false);
      }
      return;
    }

    for (auto item : elements) {
      auto newPos = pointerPos - createElementPos(item, screenSize);

      item->setFocus(item->containsPoint(newPos, item->getSize()));
      item->eventPointerMove(newPos, getSize());
    }
  }

  void ContainerElement::eventProcess(GuiEventType type) {
    for (auto item : elements) {
      item->eventProcess(type);
    }
  }

  void ContainerElement::setItemPadding(const glm::vec2& value) {
    padding = value;
  }

  void ContainerElement::setItemSpace(float value) {
    itemSpace = value;
  }

  bool ContainerElement::elementContainsPoint(elementptr_t element, const glm::vec2& point, const glm::vec2& screenSize, bool searchDown) const {
    for (auto item : elements) {
      auto newPoint = point - createElementPos(item, screenSize);
      if (item == element) {
        return item->containsPoint(newPoint, item->getSize());
      }

      if (searchDown) {
        auto cont = std::dynamic_pointer_cast<ContainerElement>(item);
        if (cont && cont->elementContainsPoint(element, newPoint, item->getSize(), searchDown))
          return true;
      }
    }
    return false;
  }

  glm::vec2 ContainerElement::createElementPos(elementptr_t element, const glm::vec2& screenSize) const {
    auto it = std::find(elements.begin(), elements.end(), element);
    if(it == elements.end())
      return glm::vec2();
    return createElementPos(std::distance(elements.begin(), it), screenSize);
  }
}