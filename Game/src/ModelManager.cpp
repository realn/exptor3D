#include "ModelManager.h"
#include "Log.h"

GLModelManager::GLModelManager( const std::string& strDataDir, CTexManager& texManager ) :
	DataDir( strDataDir ),
	TexManager( texManager )
{
}

GLModelManager::~GLModelManager()
{
	//Clear();
}

CTexManager& GLModelManager::GetTexMng()
{
	return TexManager;
}

GLModel* GLModelManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "MODELMANAGER(): Pusty ci�g znak�w!" );
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
		Log.Error( "MODELMANAGER(): Nieudane za�adowanie modelu: " + filename );
		delete Model;
		return nullptr;
	}

	AddModel( Model );
	Log.Log( "MODELMANAGER(): Dodano nowy model: " + filename );
	return Model;
}

void GLModelManager::AddModel( GLModel *Model )
{
	List.push_back( Model );
}

void GLModelManager::DeleteModel( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

GLModel* GLModelManager::GetModel( unsigned int index )
{
	if( index >= List.size() )
		return 0;

	return List[index];
}

void GLModelManager::Clear()
{
	for(unsigned i = 0; i < List.size(); i++)
	{
		delete GetModel( i );
	}
	List.clear();
}

