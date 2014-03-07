#pragma once

#include "WeaponBullet.h"
#include "ModelManager.h"

// Rakieta
class CBullRocket : 
	public CBullet
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
};

