#pragma once

#include "Item.h"
#include "GamePlayer.h"

class CItemManager
{
private:
	std::vector<CItem*> List;

public:
	CItemManager();
	~CItemManager();

	void AddBonus( CItem* Bonus );
	CItem* GetBonus( unsigned int index );
	void DeleteBonus( unsigned int index );

	void Update( CPlayer* Player, const float fTD  );
	void Render();

	unsigned int Count();
	void Clear();
};

extern CItemManager BonusMan;
