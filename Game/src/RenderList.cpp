#include "RenderList.h"

CRenderList::CRenderList( CTexManager& texManager, GLModelManager& modelManager ) :
	TexManager( texManager ),
	ModelManager( modelManager )
{
}

void	CRenderList::Add( IRenderable* pObj )
{
	if( std::find( List.cbegin(), List.cend(), pObj ) == List.cend() )
	{
		List.push_back( pObj );
		if( !pObj->IsGfxLoaded() )
			AddToLoad( pObj );
	}
}

void	CRenderList::PreLoad()
{
	if( ToLoad.empty() )
		return;

	for( unsigned i = 0; i < ToLoad.size(); i++ )
	{
		ToLoad[i]->LoadGraphic( TexManager, ModelManager );
	}

	ToLoad.clear();
}

void	CRenderList::Render()
{
	for( unsigned i = 0; i < List.size(); i++ )
	{
		if( !List[i]->IsVisible() )
			continue;

		if( !List[i]->IsGfxLoaded() )
		{
			AddToLoad( List[i] );
			continue;
		}

		List[i]->Render();
	}
}

void	CRenderList::AddToLoad( IRenderable* pObj )
{
	if( std::find( ToLoad.cbegin(), ToLoad.cend(), pObj ) == ToLoad.cend() )
		ToLoad.push_back( pObj );
}