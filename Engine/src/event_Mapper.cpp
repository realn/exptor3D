#include "event_Mapper.h"

namespace event {
  namespace {
    template<class eventid_t, class func_t, class event_t>
    void executeIfFound(const eventid_t& id, const std::map<eventid_t, func_t>& mappings, const event_t& event) {
      auto it = mappings.find(id);
      if (it != mappings.end()) {
        auto& func = it->second;

        func(event);
      }
    }
  }

  void Mapper::addAction(cb::string name, actionfunc_t&& func) {
    auto id = EventId{ EventType::Action, name };
    actionMappings[id] = std::move(func);
  }

  void Mapper::addState(cb::string name, statefunc_t&& func) {
    auto id = EventId{ EventType::State, name };
    stateMappings[id] = std::move(func);
  }

  void Mapper::addRange(cb::string name, rangefunc_t&& func) {
    auto id = EventId{ EventType::Range, name };
    rangeMappings[id] = std::move(func);
  }

  void Mapper::executeEvent(const Event& event) const {
    auto id = EventId{ event.getType(), event.getName() };
    switch (event.getType()) {
    case EventType::Action:
      executeIfFound(id, actionMappings, event.getAction());
      return;

    case EventType::State:
      executeIfFound(id, stateMappings, event.getState());
      return;

    case EventType::Range:
      executeIfFound(id, rangeMappings, event.getRange());
      return;
    }
  }
}