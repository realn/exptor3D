#include "Item.h"

#include "gui.h"

/*	KLASA BONUSÓW	*/
CItem::CItem() :
	Model( nullptr ),
	CanDelete( false ),
	type( BONUS_TYPE_UNKNOWN )
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

unsigned int CItem::GetType()
{
	return type;
}

