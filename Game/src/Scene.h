#pragma once

#include <vector>

#include "SceneEntity.h"

class CScene
{
private:
	std::vector<ISceneEntity*>	SceneList;

public:
	CScene();
	virtual ~CScene();

	virtual const bool	AddEntity( ISceneEntity* pEntity );
	virtual const bool	RemoveEntity( ISceneEntity* pEntity );

protected:
	void	FlushDeleted();
	virtual void	OnDeleteEntity( ISceneEntity* pEntity );
};