#pragma once

#include <CBCore/Defines.h>

#include "core_StrArgList.h"

namespace event {
  enum class EventType {
    Action = 0,
    State,
    Range,
  };

  class EventAction;
  class EventState;
  class EventRange;

  class Event {
  public:
    virtual ~Event() = default;

    EventType getType() const { return type; }
    const cb::string& getName() const { return name; }

    const EventAction& getAction() const;
    const EventState& getState() const;
    const EventRange& getRange() const;

  protected:
    Event(EventType type, cb::string name) : type(type), name(name) {}

  private:
    EventType type = EventType::Action;
    cb::string name;
  };

  class EventAction : public Event, public core::StrArgList {
  public:
    EventAction(cb::string name, const cb::strvector& args = cb::strvector());
    ~EventAction() override = default;
  };

  class EventState : public Event {
  public:
    EventState(cb::string name, bool state);
    ~EventState() override = default;

    bool getState() const { return state; }

  private:
    bool state = true;
  };

  class EventRange : public Event {
  public:
    EventRange(cb::string name, float value, float oldValue);
    ~EventRange() override = default;

    float getValue() const { return value; }
    float getOldValue() const { return oldValue; }

  private:
    float value = 0.0f;
    float oldValue = 0.0f;
  };
}