#pragma once

#include "WeaponProjectile.h"
#include "ModelManager.h"

// Bomba
class CBullBomb : public CProjectile
{
private:
	float BoomTime;
	float ThisTime;
	gfx::Model* Model;

public:
	CBullBomb( CActor* owner, const float damage, const glm::vec3& pos, const float time, gfx::ModelManager& modelManager );

	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
