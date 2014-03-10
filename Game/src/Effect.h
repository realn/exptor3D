#pragma once

#include "3dMath.h"
#include "Texture.h"
#include "Level.h"
#include "Renderable.h"
#include "Updateable.h"
#include "SceneEntity.h"

class CEffect :
	public IRenderable,
	public IUpdateable,
	public ISceneEntity
{
public:
	CEffect();
	virtual ~CEffect();
};

//class CEffectRay : 
//	public CEffect
//{
//private:
//	Vector3f FromPos;
//	Vector3f ToPos;
//	float Alpha;
//
//public:
//	CEffectRay( const Vector3f& origin, const Vector3f& dest );
//
//	void Render() override;
//	void Update( const float fTD ) override;
//
//	const Vector3f	GetFromPos() const;
//	const Vector3f	GetToPos() const;
//};

class CEffectExplosion : 
	public CEffect
{
private:
	Vector3f Pos;
	float Alpha;
	float Speed;
	float MaxRadius;
	float Radius;
	GLUquadric* Obj;

public:
	CEffectExplosion( const Vector3f& pos, const float radius, float speed );
	virtual ~CEffectExplosion();

	const bool	IsTransparent() const override;

	void Render() override;
	void Update( const float fTD ) override;
};

class CEffectSprite : 
	public CEffect
{
private:
	Vector3f Pos;
	CTexture* Texture;
	float Alpha;
	float Speed;
	float C[3];

public:
	CEffectSprite( const Vector3f& pos, const float R = 1.0f, const float G = 1.0f, const float B = 1.0f );
	virtual ~CEffectSprite();

	const bool	LoadGraphic( CTexManager& texManager, CModelManager& modelManager ) override;

	const bool	IsTransparent() const override;

	void Render() override;
	void Update( const float fTD ) override;
};
