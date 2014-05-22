#include "stdafx.h"
#include "UpdateList.h"

CUpdateList::CUpdateList()
{

}

CUpdateList::~CUpdateList()
{

}

void	CUpdateList::Add( IUpdateable* pObj )
{
	if( std::find( List.cbegin(), List.cend(), pObj ) == List.cend() )
		List.push_back( pObj );
}

void	CUpdateList::Remove( IUpdateable* pObj )
{
	auto it = std::find( List.cbegin(), List.cend(), pObj );
	if( it != List.cend() )
		List.erase( it );
}

void	CUpdateList::Update( const float fTD )
{
	for( unsigned i = 0; i < List.size(); i++ )
	{
		if( !List[i]->IsActive() )		
			continue;

		List[i]->Update( fTD );
	}
}