#pragma once

#include "WeaponBullet.h"

// Promieñ
class CBullRay : 
	public CBullet
{
public:
	CBullRay(CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector );
	
	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
