#pragma once

#include "WeaponBullet.h"

// Rakieta
class CBullRocket : 
	public CBullet
{
private:
	GLModel* Model;
	float Angle;
	float Sec;

public:
	CBullRocket( CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed );

	void Update( const float fTD ) override;
	void Render() override;

	void OnDelete() override;
};

