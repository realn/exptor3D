#include "ModelManager.h"
#include "Log.h"

CModelManager::CModelManager( const std::string& strDataDir, CTexManager& texManager ) :
	DataDir( strDataDir ),
	TexManager( texManager )
{
}

CModelManager::~CModelManager()
{
	//Clear();
}

CTexManager& CModelManager::GetTexMng()
{
	return TexManager;
}

GLModel* CModelManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "MODELMANAGER(): Pusty ci¹g znaków!" );
		return nullptr;
	}

	auto path = DataDir + filename;

	GLModel* Model;
	for( unsigned i = 0; i < List.size(); i++ )
	{
		Model = GetModel( i );
		if( Model->GetFile() == path )
			return Model;
	}

	Model = new GLModel;
	if( !Model->LoadModel( TexManager, path ) )
	{
		Log.Error( "MODELMANAGER(): Nieudane za³adowanie modelu: " + filename );
		delete Model;
		return nullptr;
	}

	AddModel( Model );
	Log.Log( "MODELMANAGER(): Dodano nowy model: " + filename );
	return Model;
}

void CModelManager::AddModel( GLModel *Model )
{
	List.push_back( Model );
}

void CModelManager::DeleteModel( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

GLModel* CModelManager::GetModel( unsigned int index )
{
	if( index >= List.size() )
		return 0;

	return List[index];
}

void CModelManager::Clear()
{
	for(unsigned i = 0; i < List.size(); i++)
	{
		delete GetModel( i );
	}
	List.clear();
}

