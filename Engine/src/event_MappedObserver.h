#pragma once

#include "event_Observer.h"
#include "event_Mapper.h"

namespace event {
  class MappedObserver
    : public IObserver {
  public:
    ~MappedObserver() override = default;

    virtual void	processEvent(const Event& event) override {
      mapper.executeEvent(event);
    }

  protected:
    Mapper mapper;
  };
}
