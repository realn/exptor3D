#include "stdafx.h"
#include "ItemArmor.h"

/*	BONUS - PANCERZ	*/
CItemArmor::CItemArmor( const float armor ) :
	CItem(ITEM_TYPE::ARMOR),
	ArmorValue( armor )
{
}

CItemArmor::~CItemArmor()
{
}

const bool	CItemArmor::LoadGraphic( CTexManager& texManager, CModelManager& modelManager )
{
	Model = modelManager.Get( "rocketlun-model.glm" );
	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}

const float CItemArmor::GetArmor() const
{
	return ArmorValue;
}
