#include "RenderList.h"

CRenderList::CRenderList( CTexManager& texManager, CModelManager& modelManager ) :
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

void	CRenderList::Remove( IRenderable* pObj )
{
	auto it = std::find( List.cbegin(), List.cend(), pObj );
	if( it != List.cend() )
		List.erase( it );
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

void	CRenderList::Sort()
{
	SolidList.clear();
	TransparentList.clear();

	for( unsigned i = 0; i < List.size(); i++ )
	{
		if( !List[i]->IsVisible() )
			continue;

		if( !List[i]->IsGfxLoaded() )
		{
			AddToLoad( List[i] );
			continue;
		}

		if( List[i]->IsTransparent() )
			TransparentList.push_back( List[i] );
		else
			SolidList.push_back( List[i] );
	}
}

void	CRenderList::Render(const bool solid)
{
	if( solid )
	{
		for( unsigned i = 0; i < SolidList.size(); i++ )
			SolidList[i]->Render();
	}
	else
	{
		for( unsigned i = 0; i < TransparentList.size(); i++ )
			TransparentList[i]->Render();
	}
}

void	CRenderList::AddToLoad( IRenderable* pObj )
{
	if( std::find( ToLoad.cbegin(), ToLoad.cend(), pObj ) == ToLoad.cend() )
		ToLoad.push_back( pObj );
}