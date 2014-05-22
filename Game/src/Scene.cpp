#include "stdafx.h"
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
	std::vector<ISceneEntity*> toDelete;
	for( unsigned i = SceneList.size(); i > 0; i-- )
	{
		ISceneEntity* pEnt = SceneList[i-1];
		if( !pEnt->CanDelete() )
			continue;

		this->OnDeleteEntity( pEnt );
		toDelete.push_back( pEnt );
		SceneList.erase(  SceneList.begin() + i - 1 );
	}

	for( unsigned i = 0; i < toDelete.size(); i++ )
	{
		ISceneEntity* pEnt = toDelete[i];
		pEnt->OnDelete();
	}
}

void	CScene::OnDeleteEntity( ISceneEntity* pEntity )
{
}

void	CScene::ClearScene()
{
	for( unsigned i = 0; i < SceneList.size(); i++ )
	{
		ISceneEntity* pEnt = SceneList[i];
		this->OnDeleteEntity( pEnt );
	}
	SceneList.clear();
}