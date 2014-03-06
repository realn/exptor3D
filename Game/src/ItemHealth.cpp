#include "ItemHealth.h"

/*	BONUS - ZDROWIE	*/
CItemHealth::CItemHealth( const float health ) :
	CItem(ITEM_TYPE::HEALTH),
	HealthValue( health )
{
}

const bool	CItemHealth::LoadGraphic( CTexManager& texManager, GLModelManager& modelManager )
{
	Model = modelManager.Get( "rocketlun-model.glm" );
	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}


const float CItemHealth::GetHealth() const
{
	return HealthValue;
}
