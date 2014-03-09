#pragma once

#include "GameActor.h"
#include "ModelManager.h"

class CWeapon;
class CItem;

enum class WEAPON_HAND;

#define WEAPON_COUNT 10

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
	CModelManager* ModelManager;

	WEAPON_HAND	Hand;

	// Postaæ biega?
	bool run;

	// "Krok" o jaki porusza siê postaæ chodz¹c
	float WalkStep;

	// ... i biegaj¹c
	float RunStep;
	
	// Aktualna Broñ
	unsigned int CurrWeap;
public:
	CWeapon* Weapon[WEAPON_COUNT];

	CPlayer();
	~CPlayer();

	void	Init( CModelManager& modelManager );

	void	OnDie() override;
	void	OnDead() override;

	void Render();
	void Update( const float fTD ) override;
	void ParseKeys( const bool* Keys );

	void SwichWeap( unsigned int index );

	const WEAPON_HAND GetHand() const;

	void TestWeapon( CWeapon* Weap );
	void TestBonus( CItem* Bonus );
	void ApplyNextPos();

	void ModHealth( const float mod ) override;
	void Reset() override;

	const bool	OnCollision( CObject* pObject ) override;
	const bool	OnCollision( const Vector3f& point, const Planef& plane ) override;

private:
	virtual void	SolveActions( const float fTD ) override;
};

extern CPlayer MainPlayer;
