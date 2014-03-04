#include "ItemManager.h"

#include "gui.h"

/*	MENAD¯ER BONUSÓW	*/
CItemManager::CItemManager()
{	}

CItemManager::~CItemManager()
{
	if( Count() != 0 )
		Clear();
}

void CItemManager::AddBonus(CItem *Bonus)
{
	List.push_back( Bonus );
}

CItem* CItemManager::GetBonus(unsigned int index)
{
	if( index >= Count() )
		return nullptr;
	
	return List[index];
}

void CItemManager::DeleteBonus(unsigned int index)
{
	if( index >= Count() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

unsigned int CItemManager::Count()
{
	return List.size();
}

void CItemManager::Clear()
{
	if( Count() == 0 )
		return;

	int i;
	for( i = Count()-1; i >= 0; i-- )
	{
		DeleteBonus( i );
	}
}

void CItemManager::Update( CPlayer *Player, const float fTD )
{
	if( Count() == 0 || Player == NULL )
		return;

	int i; CItem* Bonus;
	for( i = Count()-1; i >= 0; i-- )
	{
		Bonus = GetBonus( i );

		if( mathDistSq( Bonus->Pos, Player->NextPos ) > POW( 90.0f ) )
			continue;

		Bonus->Update( fTD );
		Player->TestBonus( Bonus );

		if( Bonus->CanDelete )
			DeleteBonus( i );
	}
}

void CItemManager::Render()
{
	unsigned int i;
	CItem* Bonus;
	for( i = 0; i < List.size(); i++ )
	{
		Bonus = List[i];
		if( mathDistSq( Bonus->Pos, MainPlayer.NextPos ) > POW( 90.0f ) )
			continue;
		Bonus->Render();
	}
	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		for( i = 0; i < List.size(); i++ )
		{
			Bonus = List[i];
			if( mathDistSq( Bonus->Pos, MainPlayer.NextPos ) > POW( 90.0f ) )
				continue;
			Bonus->Render();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
}

CItemManager BonusMan;
