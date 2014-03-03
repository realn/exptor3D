#pragma once

#include "WeaponBullet.h"

// Bomba
class CBullBomb : public CBullet
{
private:
	float BoomTime;
	float ThisTime;
	GLModel* Model;

public:
	CBullBomb( CActor* owner, const float damage, const Vector3f& pos, const float time );

	void Update( const float fTD ) override;
	void Render() override;

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
};
