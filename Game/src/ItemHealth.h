#pragma once

#include "Item.h"

class CItemHealth : 
	public CItem
{
private:
	float HealthValue;

public:
	CItemHealth( const float health );

	const bool	LoadGraphic( CTexManager& texManager, GLModelManager& modelManager ) override;

	const float GetHealth() const;
};
