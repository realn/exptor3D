#pragma once

#include "GameActor.h"
#include "ModelManager.h"

class weWeapon;
class CItem;

/*	KLASA POSTACI
	Postacie gry maj¹ swoj¹ binarn¹
	postaæ w taki sposób. Dziêki niej
	mo¿na ³atwo sterowaæ i wp³ywaæ na ich
	zachowanie.
*/
class CPlayer : 
	public CActor
{
private:
	GLModelManager* ModelManager;

	// Postaæ biega?
	bool run;

	// "Krok" o jaki porusza siê postaæ chodz¹c
	float WalkStep;

	// ... i biegaj¹c
	float RunStep;
	
	// Aktualna Broñ
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
