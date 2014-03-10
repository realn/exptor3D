#pragma once

#include <vector>

#include "3dMath.h"
#include "CollisionBlock.h"

class CCollision
{
public:
	enum class COLLISION_TYPE
	{
		NONE	= 0,
		SURFACE = 1,
		OBJECT	= 2,
	};

	COLLISION_TYPE	Type;
	Vector3f		Point;
	Planef			Plane;
	CObject*		pObject;

	CCollision() :
		Type( COLLISION_TYPE::NONE ), pObject( nullptr )
	{}
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
	const Vector3f	RayCast( const Vector3f& origin, const Vector3f& vector, const float step = 1.0f, const bool ignoreObjects = false ) const;
	const bool		IsClearLine( const Vector3f& origin, const Vector3f& dest, const unsigned steps = 10, const bool ignoreObjects = false ) const;

private:
	CCollisionBlock*	GetBlock( const unsigned row, const unsigned col ) const;
	const unsigned		FindBlockIndex( const Vector3f& point ) const;
	CCollisionBlock*	FindBlock( const Vector3f& point ) const;

	const bool	FindCollisionForDynamic( const CDynamic& dynamic, CCollision& outCollision, const bool ignoreObjects = false ) const;
	void	FindBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions ) const;
	void	FindFullBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collision ) const;
};