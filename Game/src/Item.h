#pragma once

#include "3dMath.h"
#include "Model.h"

#include "Renderable.h"
#include "Updateable.h"
#include "SceneEntity.h"

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
	public IUpdateable,
	public ISceneEntity
{
protected:
	const ITEM_TYPE Type;
	bool	Active;
	CModel* Model;

public:
	CItem(const ITEM_TYPE type);
	virtual ~CItem();

	virtual void	Render() override;
	virtual void	Update( const float fTD ) override;

	virtual void	SetActive( const bool set );

	const ITEM_TYPE GetType() const;
	const bool		IsActive() const;
	const bool		IsCollidable() const override;

	bool CanDelete;
};

extern const ITEM_TYPE	ParseItem(const std::string& str);