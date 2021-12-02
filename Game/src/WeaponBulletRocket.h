#pragma once

#include "WeaponProjectile.h"
#include "ModelManager.h"

// Rakieta
class CBullRocket : 
	public CProjectile
{
private:
	gfx::Model* Model;
	float Angle;
	float Sec;

public:
	CBullRocket( CActor* owner, const float damage, const glm::vec3& pos, const glm::vec3& vector, const float speed );

	const bool	LoadGraphic( gfx::TextureRepository& texManager, gfx::ModelManager& modelManager ) override;

	void Update( const float fTD ) override;
	void Render() override;

	void OnDelete() override;

	const bool OnCollision( const glm::vec3& point, const Planef& plane ) override;
};

