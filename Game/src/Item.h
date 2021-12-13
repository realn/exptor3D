#pragma once

#include "3dMath.h"
#include <gfx_Model.h>
#include <logic_SceneNode.h>

#include "Renderable.h"
#include "Updateable.h"

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
	public logic::SceneNode
{
protected:
	const ITEM_TYPE Type;
	bool	Active;
	gfx::Model* Model;

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