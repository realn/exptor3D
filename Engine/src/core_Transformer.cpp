#include "core_Transformer.h"

namespace core {
  Transformer::~Transformer() = default;

  void Transformer::setEnabled(bool val) {
    enabled = val;
  }

  void Transformer::setReverse(bool val) {
    reversed = val;
  }

  bool Transformer::isEnabled() const {
    return enabled;
  }

  bool Transformer::isReversed() const {
    return reversed;
  }

  TransformerList::~TransformerList() = default;

  void TransformerList::update(float timeDelta) {
    for (auto item : transformers) {
      item->update(timeDelta);
    }
  }

  void TransformerList::add(transformerptr_t item) {
    transformers.push_back(item);
  }
}