#include "stdafx.h"
#include "EventManager.h"

void	CEventManager::AddEvent(const CEvent& event)
{
	EventList.push_back( event );
}

void	CEventManager::ProcessEvents()
{
	for( unsigned obsIndex = 0; obsIndex < ObserverList.size(); obsIndex++ )
	{
		auto pObserver = ObserverList[obsIndex];
		for( unsigned evIndex = 0; evIndex < EventList.size(); evIndex++ )
		{
			pObserver->ProcessEvent( EventList[evIndex] );
		}
	}
	EventList.clear();
}

void	CEventManager::AddObserver( IEventObserver* pObserver )
{
	if( std::find( ObserverList.cbegin(), ObserverList.cend(), pObserver ) == ObserverList.cend() )
		ObserverList.push_back( pObserver );
}

void	CEventManager::RemoveObserver( IEventObserver* pObserver )
{
	auto it = std::find( ObserverList.cbegin(), ObserverList.cend(), pObserver );
	if( it != ObserverList.cend() )
		ObserverList.erase( it );
}