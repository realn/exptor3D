#pragma once

#include <CBCore/Defines.h>

namespace core {
  class Id {
  public:
    Id();

    bool isValid() const;

    bool operator==(const Id& other) const;
    bool operator!=(const Id& other) const;

    bool operator<(const Id& other) const;

    static Id genId();
    static Id invalid();

  private:
    explicit Id(cb::u32 value);

    cb::u32 value;
  };
}
