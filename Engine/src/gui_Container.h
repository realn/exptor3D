#pragma once

#include <memory>
#include <vector>

namespace gui {
  class Element;
  class Container {
  public:
    using elementptr_t = std::shared_ptr<Element>;

    virtual ~Container();

    void addElement(elementptr_t element);
    void clear();

  protected:
    using elements_t = std::vector<elementptr_t>;

    elements_t elements;
  };
}
