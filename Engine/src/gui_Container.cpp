#include "gui_Container.h"

namespace gui {
  Container::~Container() = default;

  void Container::addElement(elementptr_t element) {
    elements.push_back(element);
  }

  void Container::clear() {
    elements.clear();
  }

}
