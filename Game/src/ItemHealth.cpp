#include "ItemHealth.h"

/*	BONUS - ZDROWIE	*/
CItemHealth::CItemHealth( const float health, GLModel* model ) :
	CItem(ITEM_TYPE::HEALTH),
	HealthValue( health )
{
	Model = model;
}

const float CItemHealth::GetHealth() const
{
	return HealthValue;
}
