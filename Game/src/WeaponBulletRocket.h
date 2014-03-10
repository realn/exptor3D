#pragma once

#include "WeaponProjectile.h"
#include "ModelManager.h"

// Rakieta
class CBullRocket : 
	public CProjectile
{
private:
	CModel* Model;
	float Angle;
	float Sec;

public:
	CBullRocket( CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed );

	const bool	LoadGraphic( CTexManager& texManager, CModelManager& modelManager ) override;

	void Update( const float fTD ) override;
	void Render() override;

	void OnDelete() override;

	const bool OnCollision( const Vector3f& point, const Planef& plane ) override;
};

