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
	void	AddFace( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 );

	const CCollisionFace&	GetFace( unsigned index ) const;
	const unsigned	GetFaceNumber() const;

	void	AddSideBlock( CCollisionBlock* pBlock );
	const CCollisionBlock&	GetSideBlock( const unsigned index ) const;
	const unsigned	GetSideBlockNumber() const;
};