#pragma once

#include "GameActor.h"
#include "ModelManager.h"

class CWeapon;
class CItem;

enum class WEAPON_HAND;

#define WEAPON_COUNT 10

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
	CModelManager* ModelManager;

	WEAPON_HAND	Hand;

	// Posta� biega?
	bool run;

	// "Krok" o jaki porusza si� posta� chodz�c
	float WalkStep;

	// ... i biegaj�c
	float RunStep;
	
	// Aktualna Bro�
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
