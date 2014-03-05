#include "ItemArmor.h"

/*	BONUS - PANCERZ	*/
CItemArmor::CItemArmor( const float armor, GLModel* model ) :
	CItem(ITEM_TYPE::ARMOR),
	ArmorValue( armor )
{
	Model = model;
}

const float CItemArmor::GetArmor() const
{
	return ArmorValue;
}
