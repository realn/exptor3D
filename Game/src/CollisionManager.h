#pragma once

#include <vector>

#include "3dMath.h"
#include "CollisionBlock.h"

class CCollision
{
public:
	enum class COLLISION_TYPE
	{
		SURFACE = 0,
		OBJECT = 1,
	};

	COLLISION_TYPE	Type;
	Vector3f		Point;
	Planef			Plane;
	CObject*		pObject;

	CCollision( const Vector3f& point, const Planef& plane ) :
		Type( COLLISION_TYPE::SURFACE ), Point( point ), Plane( plane ), pObject( nullptr )
	{}
	CCollision( const Vector3f& point, CObject* pObject ) :
		Type( COLLISION_TYPE::OBJECT ), Point( point ), pObject( pObject )
	{}
};

class CCollisionManager
{
private:
	unsigned	Rows, Cols;
	float		BlockWidth, BlockHeight;
	std::vector<CCollisionBlock>	Blocks;

	std::vector<CObject*>	ObjectList;
	std::vector<CDynamic*>	DynamicList;
	
public:
	CCollisionManager();
	~CCollisionManager();

	void	SetBlockSize( const float width, const float depth );
	void	SetLevelSize( const unsigned rows, const unsigned cols );
	void	FindSideBlocks();

	void	AddBlock( const CCollisionBlock& Block );
	void	ClearBlocks();

	const bool	Contains(CObject* pObject) const;
	void	AddObject(CObject* pObject);
	void	DeleteObject(CObject* pObject);
	void	Clear();

	void	Solve();

private:
	CCollisionBlock*	GetBlock( const unsigned row, const unsigned col );
	const unsigned		FindBlockIndex( const Vector3f& point );
	CCollisionBlock*	FindBlock( const Vector3f& point );

	void	FindBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions );
	void	FindFullBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collision );
};