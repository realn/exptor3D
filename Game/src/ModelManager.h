#pragma once

#include "glm.h"

class GLModelManager
{
private:
	CTexManager&	TexManager;
	std::vector<GLModel*> List;

public:
	GLModelManager( CTexManager& texManager );
	~GLModelManager();

	CTexManager& GetTexMng();

	GLModel* Get( std::string filename );
	void AddModel( GLModel* Model );
	GLModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );

	void Clear();
};

