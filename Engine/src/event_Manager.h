#pragma once

#include <memory>
#include <string>
#include <vector>

#include <CBCore/Defines.h>

namespace event {
	class Event;
	class IObserver;
	class Manager {
	public:
		using eventptr_t = std::unique_ptr<Event>;
		using observerptr_t = std::shared_ptr<IObserver>;

		void addEvent(eventptr_t event);
		void addActionEvent(cb::string name, const cb::strvector& args = cb::strvector());
		void addStateEvent(cb::string name, bool state);
		void addRangeEvent(cb::string name, float value, float oldValue);

		void processEvents();

		void addObserver(observerptr_t observer);
		void removeObserver(observerptr_t observer);

	private:
		using events_t = std::vector<eventptr_t>;
		using observerweakptr_t = std::weak_ptr<IObserver>;
		using observers_t = std::vector<observerweakptr_t>;

		events_t events;
		observers_t observers;
	};
}