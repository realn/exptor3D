#include "ItemHealth.h"


/*	BONUS - ZDROWIE	*/
CItemHealth::CItemHealth( const float health, GLModel* model ) :
	HealthValue( health )
{
	Model = model;
	type = BONUS_TYPE_HEALTH;
}

const float CItemHealth::GetHealth() const
{
	return HealthValue;
}
