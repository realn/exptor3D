#pragma once

#include "Renderable.h"

class CRenderList
{
private:
	CTexManager&	TexManager;
	GLModelManager&	ModelManager;

	std::vector<IRenderable*>	List;
	std::vector<IRenderable*>	ToLoad;

public:
	CRenderList( CTexManager& texManager, GLModelManager& modelManager );
	
	void	Add( IRenderable* pObj );

	void	PreLoad();
	void	Render();

private:
	void	AddToLoad( IRenderable* pObj );
};