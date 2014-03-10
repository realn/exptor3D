#pragma once

#include "WeaponProjectile.h"

// Promie�
class CBullRay : 
	public CProjectile
{
public:
	CBullRay(CActor* owner, const float damage, const Vector3f& pos, const Vector3f& target );
	
	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
