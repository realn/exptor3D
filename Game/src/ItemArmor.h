#pragma once

#include "Item.h"

class CItemArmor : 
	public CItem
{
private:
	float ArmorValue;

public:
	CItemArmor( const float armor );
	virtual ~CItemArmor();
	
	const bool	LoadGraphic( gfx::TextureRepository& texManager, CModelManager& modelManager ) override;

	const float GetArmor() const;
};
