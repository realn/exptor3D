#pragma once

#include "Item.h"

class CItemHealth : 
	public CItem
{
private:
	float HealthValue;

public:
	CItemHealth( const float health, GLModel* model );

	const float GetHealth() const;
};
