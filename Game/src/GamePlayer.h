#pragma once

#include "GameActor.h"
#include "ModelManager.h"

class weWeapon;
class CItem;

/*	KLASA POSTACI
	Postacie gry maj� swoj� binarn�
	posta� w taki spos�b. Dzi�ki niej
	mo�na �atwo sterowa� i wp�ywa� na ich
	zachowanie.
*/
class CPlayer : 
	public CActor
{
private:
	GLModelManager* ModelManager;

	// Posta� biega?
	bool run;

	// "Krok" o jaki porusza si� posta� chodz�c
	float WalkStep;

	// ... i biegaj�c
	float RunStep;
	
	// Aktualna Bro�
	unsigned int CurrWeap;
	unsigned int Hand;
public:
	weWeapon* Weapon[10];

	CPlayer();
	~CPlayer();

	void	Init( GLModelManager& modelManager );

	void	OnDie() override;
	void	OnDead() override;

	void Render();
	void Update( const float fTD ) override;
	void Update( bool* Keys, const float fTD );

	void Move(unsigned uFlags, const float fTD ) override;

	void SwichWeap( unsigned int index );

	unsigned int GetHand();

	void TestWeapon( weWeapon* Weap );
	void TestBonus( CItem* Bonus );
	void ApplyNextPos();

	void ModHealth( const float mod ) override;
	void Reset() override;
};

extern CPlayer MainPlayer;
