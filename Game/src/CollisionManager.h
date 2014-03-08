#pragma once

#include <vector>

#include "3dMath.h"
#include "CollisionBlock.h"

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

	const bool	SolveBlockCollision( const CCollisionBlock& block, const CDynamic& dynamic, Vector3f& outPoint, Planef& outPlane );
	const bool	SolveFullBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, Vector3f& outPoint, Planef& outPlane );
};