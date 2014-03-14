#pragma once

#include "Model.h"

class CModelManager
{
private:
	const std::string	DataDir;
	CTexManager&		TexManager;
	std::vector<CModel*> List;

public:
	CModelManager( const std::string& strDataDir, CTexManager& texManager );
	~CModelManager();

	CTexManager& GetTexMng();

	CModel* Get( const std::string& filename );
	void AddModel( CModel* Model );
	CModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );

	void Clear();

private:
	CModelManager( const CModelManager& model ) : TexManager( model.TexManager )
	{
		throw std::exception( "COPY ERROR" );
	}
	void operator=( const CModelManager& model )
	{
		throw std::exception( "ASSIGN ERROR" );
	}
};

