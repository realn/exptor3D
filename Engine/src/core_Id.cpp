#include "core_Id.h"

namespace core {
  static const auto CORE_ID_INVALID = 0;
  static cb::u32 CORE_ID_COUNTER = 1;

  Id::Id() : value(CORE_ID_INVALID){}

  bool Id::isValid() const {
    return false;
  }

  bool Id::operator==(const Id& other) const {
    return false;
  }

  bool Id::operator!=(const Id& other) const {
    return false;
  }

  bool Id::operator<(const Id& other) const {
      return value < other.value;
  }

  Id Id::genId() {
    return Id(CORE_ID_COUNTER++);
  }
  Id Id::invalid() {
    return Id(CORE_ID_INVALID);
  }

  Id::Id(cb::u32 value) : value(value) {}
}