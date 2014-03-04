#pragma once

#include "Item.h"

class CItemAmmo : 
	public CItem
{
private:
	unsigned WeapType;
	unsigned AmmoCount;

public:
	CItemAmmo( const unsigned weapType, const unsigned ammoCount, GLModel* model );

	const unsigned GetWeapType() const;
	const unsigned GetAmmoCount() const;
};
