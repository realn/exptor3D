#pragma once

#include <vector>

#include "CollisionFace.h"

class CCollisionBlock
{
private:
	unsigned	Index;
	std::vector<CCollisionFace>		FaceList;
	std::vector<CCollisionBlock*>	SideBlocks;

public:
	CCollisionBlock();
	CCollisionBlock( const unsigned index );

	void	AddFace( const CCollisionFace& face );
	void	AddFace( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 );

	const CCollisionFace&	GetFace( unsigned index ) const;
	const unsigned	GetFaceNumber() const;

	void	AddSideBlock( CCollisionBlock* pBlock );
	const CCollisionBlock&	GetSideBlock( const unsigned index ) const;
	const unsigned	GetSideBlockNumber() const;
};