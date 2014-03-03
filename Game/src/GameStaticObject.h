#pragma once

#include "3dMath.h"
#include "glm.h"

/*	KLASA gameStatObj
*/
class gameStatObj : 
	public CObject
{
private:
	GLModel*	Model;
	std::string	file;

	std::string GetStr( FILE* fp );
public:
	gameStatObj()
	{	file = "-";	};

	void Update( const float fTD );
	void Render();

	bool LoadObj( std::string filename );
};

