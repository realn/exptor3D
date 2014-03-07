#pragma once

#include "Texture.h"
#include "ModelManager.h"

class IRenderable
{
protected:
	bool	GfxLoaded;
	bool	Visible;

public:
	IRenderable();
	virtual ~IRenderable();

	virtual void	Render() = 0;

	virtual const bool	LoadGraphic( CTexManager& texManager, CModelManager& modelManager );

	virtual void	SetVisible( const bool );
	virtual void	SetGfxLoaded( const bool );

	const bool	IsVisible() const;
	const bool	IsGfxLoaded() const;
	const bool	IsTransparent() const;
};