#pragma once

#include <vector>

#include "Updateable.h"

class CUpdateList
{
private:
	std::vector<IUpdateable*>	List;

public:
	CUpdateList();
	~CUpdateList();

	void	Add( IUpdateable* pObj );
	void	Remove( IUpdateable* pObj );

	void	Update( const float fTD );
};