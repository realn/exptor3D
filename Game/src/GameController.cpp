#include "stdafx.h"
#include "GameController.h"

CControllerList::CControllerList()
{
}

CControllerList::~CControllerList()
{
	Clear();
}

void	CControllerList::AddController( IController* pController )
{
	if( std::find( List.cbegin(), List.cend(), pController ) == List.cend() )
		List.push_back( pController );
}

void	CControllerList::RemoveController( IController* pController )
{
	auto it = std::find( List.cbegin(), List.cend(), pController );
	if( it != List.cend() )
		List.erase( it );
}

void	CControllerList::Clear()
{
	for( unsigned i = 0; i < List.size(); i++ )
		delete List[i];
	List.clear();
}

void	CControllerList::Update()
{
	for( unsigned i = 0; i < List.size(); i++ )
		List[i]->Update();
}