#pragma once

#include <vector>

#include "3dMath.h"
#include "GameEntity.h"

class CObjectManager
{
private:
	std::vector<CObject*>	m_objectList;
	std::vector<CDynamic*>	m_dynamicList;
	std::vector<IEntity*>	m_entityList;

public:
	CObjectManager();

	const bool	Contains(CObject* pObject) const;
	void	AddObject(CObject* pObject);
	void	DeleteObject(CObject* pObject);
	void	Clear();

	void	Update(const float fTD);
};