#include "event_Event.h"

namespace event {
  const EventAction& Event::getAction() const {
    return dynamic_cast<const EventAction&>(*this);
  }
  const EventState& Event::getState() const {
    return dynamic_cast<const EventState&>(*this);
  }
  const EventRange& Event::getRange() const {
    return dynamic_cast<const EventRange&>(*this);
  }

  EventAction::EventAction(cb::string name, const cb::strvector& args)
    : Event(EventType::Action, name), args(args)
  {
  }

  EventState::EventState(cb::string name, bool state) 
    : Event(EventType::State, name), state(state)
  {
  }
  
  EventRange::EventRange(cb::string name, float value, float oldValue)
    : Event(EventType::Range, name), value(value), oldValue(oldValue)
  {
  }
}