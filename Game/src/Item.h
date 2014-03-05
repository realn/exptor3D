#pragma once

#include "3dMath.h"
#include "GameEntity.h"
#include "glm.h"

#include "Renderable.h"

enum class ITEM_TYPE
{
	UNKNOWN	= 0,
	AMMO	= 1,
	HEALTH	= 2,
	ARMOR	= 3,
	WEAPON	= 5,
};

class CItem : 
	public CObject,
	public IRenderable,
	public IEntity
{
protected:
	const ITEM_TYPE Type;
	GLModel* Model;

public:
	CItem(const ITEM_TYPE type);
	virtual ~CItem();

	virtual void Render() override;
	virtual void Update( const float fTD ) override;

	const ITEM_TYPE GetType() const;

	bool CanDelete;
};

extern const ITEM_TYPE	ParseItem(const std::string& str);