#include "Item.h"

#include "gui.h"

/*	KLASA BONUSÓW	*/
CItem::CItem(const ITEM_TYPE type) :
	Model( nullptr ),
	CanDelete( false ),
	Type( type )
{
}

CItem::~CItem()
{
}

void CItem::Render()
{
	if( Model == nullptr )
		return;

	glPushMatrix();
	
	glTranslatef( Pos.X, Pos.Y, Pos.Z );
	glRotatef( Angle, 0.0f, 1.0f, 0.0f );
	Model->CallObject( 0 );
	
	glPopMatrix();
}

void CItem::Update( const float fTD )
{
	Angle += fTD * GUI.GetSpeed();
	Angle = SwapAngle( Angle );
}

const ITEM_TYPE CItem::GetType() const
{
	return Type;
}

//=====================================

const ITEM_TYPE	ParseItem(const std::string& str)
{
	if(str == "AMMO")	return ITEM_TYPE::AMMO;
	if(str == "HEALTH")	return ITEM_TYPE::HEALTH;
	if(str == "ARMOR")	return ITEM_TYPE::ARMOR;
	if(str == "WEAPON")	return ITEM_TYPE::WEAPON;

	return ITEM_TYPE::WEAPON;
}