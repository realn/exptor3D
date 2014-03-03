#pragma once

#include "WeaponBullet.h"

class CBullSaw : 
	public CBullet
{
public:
	CBullSaw( CActor* owner, const float damage, const Vector3f& pos );

	void Update( const float fTD ) override;
	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
