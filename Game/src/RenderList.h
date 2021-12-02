#pragma once

#include "Renderable.h"

class CRenderList
{
private:
	gfx::TextureRepository&	TexManager;
	gfx::ModelManager&	ModelManager;

	std::vector<IRenderable*>	List;
	std::vector<IRenderable*>	ToLoad;

	std::vector<IRenderable*>	SolidList;
	std::vector<IRenderable*>	TransparentList;

public:
	CRenderList( gfx::TextureRepository& texManager, gfx::ModelManager& modelManager );
	
	void	Add( IRenderable* pObj );
	void	Remove( IRenderable* pObj );

	void	PreLoad();
	void	Sort();
	void	Render(const bool solid);

private:
	void	AddToLoad( IRenderable* pObj );
};