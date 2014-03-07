#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

const bool	CScene::AddEntity( ISceneEntity* pEntity )
{
	if( std::find( SceneList.cbegin(), SceneList.cend(), pEntity ) != SceneList.cend() )
		return false;

	SceneList.push_back( pEntity );
	return true;
}

const bool	CScene::RemoveEntity( ISceneEntity* pEntity )
{
	auto it = std::find( SceneList.cbegin(), SceneList.cend(), pEntity );
	if( it == SceneList.cend() )
		return false;

	SceneList.erase( it );
	return true;
}

void	CScene::FlushDeleted()
{
	for( auto it = SceneList.crbegin(); it != SceneList.crend(); it++ )
	{
		if( !(*it)->CanDelete() )
			continue;

		(*it)->OnDelete();
		this->OnDeleteEntity( *it );

		delete (*it);
		SceneList.erase( (it+1).base() );
	}
}

void	CScene::OnDeleteEntity( ISceneEntity* pEntity )
{
}