#pragma once

#include "WeaponProjectile.h"
#include "ModelManager.h"

// Bomba
class CBullBomb : public CProjectile
{
private:
	float BoomTime;
	float ThisTime;
	CModel* Model;

public:
	CBullBomb( CActor* owner, const float damage, const Vector3f& pos, const float time, CModelManager& modelManager );

	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
