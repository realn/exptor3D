#pragma once

#include <string>
#include <vector>

class CEvent
{
public:
	unsigned		Type;
	unsigned char	Data[64];
};

class IEventObserver
{
public:
	virtual void	ProcessEvent(CEvent& event) = 0;
};

class CEventManager
{
private:
	std::vector<CEvent>	EventList;
	std::vector<IEventObserver*>	ObserverList;

public:

	void	AddEvent(const CEvent& event);

	void	ProcessEvents();

	void	AddObserver( IEventObserver* pObserver );
	void	RemoveObserver( IEventObserver* pObserver );
};