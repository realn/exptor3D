#include "ItemArmor.h"

/*	BONUS - PANCERZ	*/
CItemArmor::CItemArmor( const float armor ) :
	CItem(ITEM_TYPE::ARMOR),
	ArmorValue( armor )
{
}

const bool	CItemArmor::LoadGraphic( CTexManager& texManager, GLModelManager& modelManager )
{
	Model = modelManager.Get( "rocketlun-model.glm" );
	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}

const float CItemArmor::GetArmor() const
{
	return ArmorValue;
}
