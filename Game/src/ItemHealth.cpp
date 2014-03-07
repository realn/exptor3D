#include "ItemHealth.h"

/*	BONUS - ZDROWIE	*/
CItemHealth::CItemHealth( const float health ) :
	CItem(ITEM_TYPE::HEALTH),
	HealthValue( health )
{
}

CItemHealth::~CItemHealth()
{
}

const bool	CItemHealth::LoadGraphic( CTexManager& texManager, CModelManager& modelManager )
{
	Model = modelManager.Get( "rocketlun-model.glm" );
	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}


const float CItemHealth::GetHealth() const
{
	return HealthValue;
}
