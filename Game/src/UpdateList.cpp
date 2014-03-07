#include "UpdateList.h"

CUpdateList::CUpdateList()
{

}

CUpdateList::~CUpdateList()
{

}

void	CUpdateList::Add(IUpdateable* pObj)
{
	if( std::find( List.cbegin(), List.cend(), pObj ) == List.cend() )
		List.push_back( pObj );
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