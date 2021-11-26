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

void	CCollisionBlock::AddFace( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 )
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