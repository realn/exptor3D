#pragma once

#include "Item.h"

class CItemArmor : 
	public CItem
{
private:
	float ArmorValue;

public:
	CItemArmor( const float armor, GLModel* model );
	
	const float GetArmor() const;
};
