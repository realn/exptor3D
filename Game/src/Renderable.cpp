#include "Renderable.h"

IRenderable::IRenderable() :
	Visible( true ),
	GfxLoaded( false )
{
}

IRenderable::~IRenderable()
{
}

const bool	IRenderable::LoadGraphic( CTexManager& texManager, GLModelManager& modelManager )
{
	return true;
}

void	IRenderable::SetVisible( const bool visible )
{
	Visible = visible;
}

void	IRenderable::SetGfxLoaded( const bool loaded )
{
	GfxLoaded = loaded;
}

const bool	IRenderable::IsVisible() const
{
	return Visible;
}

const bool	IRenderable::IsGfxLoaded() const
{
	return GfxLoaded;
}