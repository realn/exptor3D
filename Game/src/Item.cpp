#include <CBGL/COpenGL.h>

#include "Item.h"

/*	KLASA BONUSÓW	*/
CItem::CItem(const ITEM_TYPE type) :
	Model( nullptr ),
	CanDelete( false ),
	Type( type ),
	Active( true )
{
	Radius = 3.0f;
	GfxLoaded = false;
}

CItem::~CItem()
{
}

void CItem::Render()
{
	if( !Active )
		return;

	if( Model == nullptr )
		return;

	glPushMatrix();
	
	glTranslatef( Pos.x, Pos.y - 3.0f, -Pos.z );
	glRotatef( Angle, 0.0f, 1.0f, 0.0f );
	Model->RenderObject( 0 );
	
	glPopMatrix();
}

void CItem::Update( const float fTD )
{
	if( !Active )
		return;

	Angle += fTD * 60.0f;
	Angle = SwapAngle( Angle );
}

void	CItem::SetActive( const bool set )
{
	Active = set;
}

const ITEM_TYPE CItem::GetType() const
{
	return Type;
}

const bool	CItem::IsActive() const
{
	return Active;
}

const bool	CItem::IsCollidable() const
{
	return Active;
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