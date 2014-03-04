#include "ModelManager.h"
#include "Log.h"

GLModelManager::GLModelManager( CTexManager& texManager ) :
	TexManager( texManager )
{
}

GLModelManager::~GLModelManager()
{
	//Clear();
}

GLModel* GLModelManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "MODELMANAGER(): Pusty ci¹g znaków!" );
		return 0;
	}

	int i;
	GLModel* Model;
	for( i = 0; i < List.size(); i++ )
	{
		Model = GetModel( i );
		if( Model->GetFile() == filename )
			return Model;
	}

	Model = new GLModel;
	if( !Model->LoadModel( TexManager, filename ) )
	{
		Log.Error( "MODELMANAGER(): Nieudane za³adowanie modelu: " + filename );
		delete Model;
		return 0;
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

