#pragma once

#include "WeaponProjectile.h"

// Promieñ
class CBullRay : 
	public CProjectile
{
public:
	CBullRay(CActor* owner, const float damage, const glm::vec3& pos, const glm::vec3& target );
	
	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
