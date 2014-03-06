#pragma once

#include "WeaponBullet.h"

// Wybuch
class CBullExplosion : 
	public CBullet
{
private:
	float Speed;
	float Power;
	float thisPower;

public:
	CBullExplosion( CActor* owner, const float damage, const Vector3f& pos, const float radius, const float speed );

	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};

