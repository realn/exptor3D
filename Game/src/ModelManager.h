#pragma once

#include "glm.h"

class CModelManager
{
private:
	const std::string	DataDir;
	CTexManager&		TexManager;
	std::vector<GLModel*> List;

public:
	CModelManager( const std::string& strDataDir, CTexManager& texManager );
	~CModelManager();

	CTexManager& GetTexMng();

	GLModel* Get( std::string filename );
	void AddModel( GLModel* Model );
	GLModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );

	void Clear();
};

