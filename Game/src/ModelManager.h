#pragma once

#include "glm.h"

class GLModelManager
{
private:
	const std::string	DataDir;
	CTexManager&		TexManager;
	std::vector<GLModel*> List;

public:
	GLModelManager( const std::string& strDataDir, CTexManager& texManager );
	~GLModelManager();

	CTexManager& GetTexMng();

	GLModel* Get( std::string filename );
	void AddModel( GLModel* Model );
	GLModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );

	void Clear();
};

