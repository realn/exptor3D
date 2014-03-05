#pragma once

#include <vector>

#include "3dMath.h"

class CCollisionManager
{
private:
	std::vector<CObject*>	ObjectList;
	std::vector<CDynamic*>	DynamicList;
	
public:
	CCollisionManager();
	~CCollisionManager();

	const bool	Contains(CObject* pObject) const;
	void	AddObject(CObject* pObject);
	void	DeleteObject(CObject* pObject);
	void	Clear();

	void	Solve();
};