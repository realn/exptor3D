#pragma once

#include <map>
#include <functional>

#include <CBCore/Defines.h>

#include "event_Event.h"

namespace event {
  class Mapper {
  public:
    using actionfunc_t = std::function<void(const EventAction&)>;
    using statefunc_t = std::function<void(const EventState&)>;
    using rangefunc_t = std::function<void(const EventRange&)>;

    void addAction(cb::string name, actionfunc_t&& func);
    void addState(cb::string name, statefunc_t&& func);
    void addRange(cb::string name, rangefunc_t&& func);

    void executeEvent(const Event& event) const;

  private:
    using EventId = std::pair<EventType, cb::string>;
    using actionmappings_t = std::map<EventId, actionfunc_t>;
    using statemappings_t = std::map<EventId, statefunc_t>;
    using rangemappings_t = std::map<EventId, rangefunc_t>;

    actionmappings_t actionMappings;
    statemappings_t stateMappings;
    rangemappings_t rangeMappings;
  };
}
