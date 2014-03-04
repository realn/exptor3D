#pragma once

#include "3dMath.h"
#include "GameEntity.h"
#include "glm.h"

#define BONUS_TYPE_UNKNOWN	0
#define BONUS_TYPE_AMMO		1
#define BONUS_TYPE_HEALTH	2
#define	BONUS_TYPE_ARMOR	3

class CItem : 
	public CObject,
	public IEntity
{
protected:
	unsigned int type;
	GLModel* Model;

public:
	CItem();

	virtual void Render() override;
	virtual void Update( const float fTD ) override;
	unsigned int GetType();

	bool CanDelete;
};
