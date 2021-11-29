#include "CollisionBlock.h"

CCollisionBlock::CCollisionBlock() :
	Index( 0 )
{
}

CCollisionBlock::CCollisionBlock( const unsigned index ) :
	Index( index )
{
}


void	CCollisionBlock::AddFace( const CCollisionFace& face )
{
	FaceList.push_back( face );
}

void	CCollisionBlock::AddFace( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 )
{
	FaceList.push_back( CCollisionFace( v1, v2, v3, v4 ) );
}

const CCollisionFace&	CCollisionBlock::GetFace( const unsigned index ) const
{
	return FaceList[index];
}

const unsigned	CCollisionBlock::GetFaceNumber() const
{
	return static_cast<unsigned>(FaceList.size());
}

void	CCollisionBlock::AddSideBlock( CCollisionBlock* pBlock )
{
	if( pBlock == nullptr )
		return;

	SideBlocks.push_back( pBlock );
}

const CCollisionBlock&	CCollisionBlock::GetSideBlock( const unsigned index ) const
{
	return *SideBlocks[index];
}

const unsigned	CCollisionBlock::GetSideBlockNumber() const
{
	return static_cast<unsigned>(SideBlocks.size());
}