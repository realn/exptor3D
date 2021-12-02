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
	gfx::ModelManager& ModelManager;

	WEAPON_HAND	Hand;

	// Posta� biega?
	bool Run;

	// "Krok" o jaki porusza si� posta� chodz�c
	float WalkStep;

	// ... i biegaj�c
	float RunStep;
	
	// Aktualna Bro�
	unsigned CurrWeap;
	CWeapon* Weapon[WEAPON_COUNT];

public:
	CPlayer( gfx::ModelManager& modelManager );
	~CPlayer();

	void	OnDie() override;
	void	OnDead() override;

	void	Render();
	void	Update( const float fTD ) override;

	void	SwichWeap( const unsigned index );
	void	SetMoveSpeed( const bool run );

	const WEAPON_HAND	GetHand() const;


	void	ModHealth( const float mod ) override;
	void	Reset() override;

	const bool	OnCollision( CObject* pObject ) override;
	const bool	OnCollision( const glm::vec3& point, const Planef& plane ) override;

private:
	const bool	ProcessItem( CItem* pItem );

	virtual void	SolveActions( const float fTD ) override;
};