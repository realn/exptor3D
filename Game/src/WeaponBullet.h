#pragma once

#include "3dMath.h"
#include "GameActor.h"
#include "Renderable.h"

#define BULLET_TYPE_NORMAL	0
#define BULLET_TYPE_RAY		1
#define BULLET_TYPE_ROCKET	2
#define	BULLET_TYPE_EXPLODE	3
#define BULLET_TYPE_BOMB	4
#define BULLET_TYPE_SAW		5

/*	KLASY POCISKÓW	*/
class CBullet : 
	public CDynamic,
	public IUpdateable,
	public IRenderable
{
protected:
	CActor* Owner;
	unsigned int Type;
	Vector3f Veloc;
	bool DoDelete;
	float Damage;
	GLUquadric* obj;

public:
	CBullet(CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed);
	virtual ~CBullet();

	CActor*	GetOwner() const;

	virtual void Update( const float fTD ) override;
	virtual void Render() override;

	virtual float DoTest( CDynamic* Dum, float Armor = 0.0f );

	virtual void OnDelete();

	bool CanDelete;
	bool Visible;
};
