#pragma once

namespace event {
	class Event;
	class IObserver {
	public:
		virtual ~IObserver() = default;

		virtual void	processEvent(const Event& event) = 0;
	};
}
