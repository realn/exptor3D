#pragma once

#include "3dMath.h"
#include "glm.h"
#include "ModelManager.h"

#include "GameEntity.h"

/*	KLASA gameStatObj
*/
class gameStatObj : 
	public IEntity,
	public CObject
{
private:
	GLModel*	Model;
	std::string	file;

	std::string GetStr( FILE* fp );
public:
	gameStatObj()
	{	file = "-";	};

	void Update( const float fTD ) override;
	void Render() override;

	bool LoadObj( GLModelManager& modelManager, const std::string& filename );
};

