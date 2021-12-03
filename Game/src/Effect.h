#pragma once

#include <gfx_Texture.h>
#include <gfx_TextureRepository.h>

#include "3dMath.h"
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
//	glm::vec3 FromPos;
//	glm::vec3 ToPos;
//	float Alpha;
//
//public:
//	CEffectRay( const glm::vec3& origin, const glm::vec3& dest );
//
//	void Render() override;
//	void Update( const float fTD ) override;
//
//	const glm::vec3	GetFromPos() const;
//	const glm::vec3	GetToPos() const;
//};

class CEffectExplosion : 
	public CEffect
{
private:
	glm::vec3 Pos;
	float Alpha;
	float Speed;
	float MaxRadius;
	float Radius;
	GLUquadric* Obj;

public:
	CEffectExplosion( const glm::vec3& pos, const float radius, float speed );
	virtual ~CEffectExplosion();

	const bool	IsTransparent() const override;

	void Render() override;
	void Update( const float fTD ) override;
};

class CEffectSprite : 
	public CEffect
{
private:
	glm::vec3 Pos;
	std::shared_ptr<gfx::Texture> Texture;
	float Alpha;
	float Speed;
	float C[3];

public:
	CEffectSprite( const glm::vec3& pos, const float R = 1.0f, const float G = 1.0f, const float B = 1.0f );
	virtual ~CEffectSprite();

	const bool	LoadGraphic( gfx::TextureRepository& texManager, gfx::ModelManager& modelManager ) override;

	const bool	IsTransparent() const override;

	void Render() override;
	void Update( const float fTD ) override;
};
