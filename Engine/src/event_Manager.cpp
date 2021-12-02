#include <algorithm>

#include "event_Event.h"
#include "event_Observer.h"
#include "event_Manager.h"

namespace event {
  void	Manager::addEvent(eventptr_t event) {
    events.push_back(std::move(event));
  }

  void Manager::addActionEvent(cb::string name, const cb::strvector& args) {
    addEvent(std::make_unique<event::EventAction>(name, args));
  }

  void Manager::addStateEvent(cb::string name, bool state) {
    addEvent(std::make_unique<event::EventState>(name, state));
  }

  void Manager::addRangeEvent(cb::string name, float value, float oldValue) {
    addEvent(std::make_unique<event::EventRange>(name, value, oldValue));
  }

  void	Manager::processEvents() {
    auto obs = observers;
    auto evts = std::move(events);
    events.clear();

    auto it = obs.begin();
    while (it != obs.end()) {
      auto observer = it->lock();
      if (!observer) {
        it = obs.erase(it);
        continue;
      }

      for (const auto& event : evts) {
        observer->processEvent(*event);
      }

      it++;
    }
  }

  void	Manager::addObserver(observerptr_t observer) {
    if (std::find_if(observers.begin(), observers.end(), [&](observerweakptr_t item) { return item.lock() == observer; }) == observers.end())
      observers.push_back(observer);
  }

  void	Manager::removeObserver(observerptr_t observer) {
    auto it = std::find_if(observers.begin(), observers.end(), [&](observerweakptr_t item) { return item.lock() == observer; });
    if (it != observers.end())
      observers.erase(it);
  }
}