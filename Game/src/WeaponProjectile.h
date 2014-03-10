#pragma once

#include "3dMath.h"
#include "GameActor.h"
#include "Renderable.h"
#include "SceneEntity.h"

enum class PROJECTILE_TYPE
{
	UNKNOWN = 0,
	BULLET,
	RAY,
	ROCKET,
	EXPLOSION,
	BOMB,
	MELEE,
};

/*	KLASY POCISKÓW	*/
class CProjectile : 
	public CDynamic,
	public IUpdateable,
	public IRenderable,
	public ISceneEntity
{
protected:
	const PROJECTILE_TYPE Type;
	CActor* Owner;
	Vector3f Veloc;
	float Damage;
	GLUquadric* obj;

public:
	CProjectile(const PROJECTILE_TYPE type, CActor* owner, const float damage, const Vector3f& origin, const Vector3f& vector, const float speed );
	virtual ~CProjectile();

	CActor*	GetOwner() const;

	virtual void Update( const float fTD ) override;
	virtual void Render() override;

	virtual float DoTest( CDynamic* Dum, float Armor = 0.0f );

	virtual void OnDelete() override;
};
