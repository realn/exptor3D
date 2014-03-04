#include "ItemArmor.h"

/*	BONUS - PANCERZ	*/
CItemArmor::CItemArmor( const float armor, GLModel* model ) :
	ArmorValue( armor )
{
	Model = model;
	type = BONUS_TYPE_ARMOR;
}

const float CItemArmor::GetArmor() const
{
	return ArmorValue;
}
