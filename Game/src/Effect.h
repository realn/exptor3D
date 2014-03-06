#pragma once

#include "3dMath.h"
#include "Texture.h"
#include "Level.h"

class CEffect
{
public:
	CEffect();
	virtual ~CEffect();

	virtual void Update( const float fTD ) = 0;
	virtual void Render() = 0;

	bool Visible;
	bool CanDelete;
};

class CEffectRay : 
	public CEffect
{
private:
	Vector3f FromPos;
	Vector3f ToPos;
	float Alpha;

public:
	CEffectRay( const CLevel& level, const Vector3f& pos, const Vector3f& vector );

	void Render() override;
	void Update( const float fTD ) override;

	const Vector3f	GetFromPos() const;
	const Vector3f	GetToPos() const;
};

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
	CEffectSprite( CTexManager& texManager, const Vector3f& pos, const float R = 1.0f, const float G = 1.0f, const float B = 1.0f );

	void Render() override;
	void Update( const float fTD ) override;
};
