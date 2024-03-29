#include <glm/glm.hpp>

#include "CollisionManager.h"

CCollisionManager::CCollisionManager() : 
	Cols( 0 ),
	Rows( 0 ),
	BlockWidth( 0.0f ),
	BlockHeight( 0.0f )
{
}

CCollisionManager::~CCollisionManager()
{
}

void	CCollisionManager::SetBlockSize( const float width, const float height )
{
	BlockWidth = width;
	BlockHeight = height;
}

void	CCollisionManager::SetLevelSize( const unsigned rows, const unsigned cols )
{
	Rows = rows;
	Cols = cols;
}

void	CCollisionManager::AddBlock( const CCollisionBlock& block )
{
	Blocks.push_back( block );
}

void	CCollisionManager::ClearBlocks()
{
	Blocks.clear();
}

void	CCollisionManager::FindSideBlocks()
{
	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
		{
			CCollisionBlock* pBlock = GetBlock( row, col );
			if( pBlock == nullptr )
				continue;

			pBlock->AddSideBlock( GetBlock( row - 1, col + 1 ) );
			pBlock->AddSideBlock( GetBlock( row + 0, col + 1 ) );
			pBlock->AddSideBlock( GetBlock( row + 1, col + 1 ) );

			pBlock->AddSideBlock( GetBlock( row - 1, col + 0 ) );
			pBlock->AddSideBlock( GetBlock( row + 1, col + 0 ) );

			pBlock->AddSideBlock( GetBlock( row - 1, col - 1 ) );
			pBlock->AddSideBlock( GetBlock( row + 0, col - 1 ) );
			pBlock->AddSideBlock( GetBlock( row + 1, col - 1 ) );
		}
	}
}

const bool	CCollisionManager::Contains(CObject* pObject) const
{
	if( std::find(ObjectList.cbegin(), ObjectList.cend(), pObject) != ObjectList.cend())
		return true;

	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
	{
		if( std::find(DynamicList.cbegin(), DynamicList.cend(), pDynamic) != DynamicList.cend())
			return true;
	}
	return false;
}

void	CCollisionManager::AddObject(CObject* pObject)
{
	if( Contains(pObject) )
		return;

	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
		DynamicList.push_back(pDynamic);
	else
		ObjectList.push_back(pObject);
}

void	CCollisionManager::DeleteObject(CObject* pObject)
{
	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
	{
		auto index = std::find(DynamicList.begin(), DynamicList.end(), pDynamic);
		if(index != DynamicList.end())
			DynamicList.erase(index);
	}
	else
	{
		auto index = std::find(ObjectList.begin(), ObjectList.end(), pObject);
		if(index != ObjectList.end())
			ObjectList.erase(index);
	}
}

void	CCollisionManager::Clear()
{
	DynamicList.clear();
	ObjectList.clear();
}

void	CCollisionManager::Solve()
{
	for( unsigned dyn = 0; dyn < DynamicList.size(); dyn++ )
	{
		CDynamic* pDynamic = DynamicList[dyn];

		CCollision collision;
		if( FindCollisionForDynamic( *pDynamic, collision, false ) )
		{
			switch (collision.Type)
			{
			case CCollision::COLLISION_TYPE::SURFACE:
				if( pDynamic->OnCollision( collision.Point, collision.Plane ) )
					pDynamic->NextPos = collision.Point + collision.Plane.Normal * pDynamic->Radius;
				break;

			case CCollision::COLLISION_TYPE::OBJECT:
				if( pDynamic->OnCollision( collision.pObject ) )
					pDynamic->NextPos = collision.Point + pDynamic->Vector * pDynamic->Radius;
				break;

			default:
				break;
			}
		}
	}
}

const glm::vec3	CCollisionManager::RayCast( const glm::vec3& origin, const glm::vec3& vector, const float step , const bool ignoreObjects ) const
{
	CDynamic dynamic( 0.1f );

	dynamic.Pos = origin;
	dynamic.Vector = vector;
	dynamic.NextPos = origin + vector * step;
	
	do
	{
		if( FindBlock( dynamic.Pos ) == nullptr )
			return dynamic.Pos;

		CCollision collision;
		if( FindCollisionForDynamic( dynamic, collision, ignoreObjects ) )
			return collision.Point;

		dynamic.Pos = dynamic.NextPos;
		dynamic.NextPos = dynamic.Pos + vector * step;
	}
	while( true );

	return origin;
}

const bool	CCollisionManager::IsClearLine( const glm::vec3& origin, const glm::vec3& dest, const unsigned steps, const bool ignoreObjects ) const
{
	CDynamic dynamic( 0.1f );

	float singleStep = glm::length(dest - origin) / (float)steps;
	dynamic.Pos = origin;
	dynamic.Vector = glm::normalize(dest - origin);
	dynamic.NextPos = origin + dynamic.Vector * singleStep;

	for( unsigned i = 0; i < steps - 1; i++ )
	{
		CCollision collision;
		if( FindCollisionForDynamic( dynamic, collision, ignoreObjects ) )
			return false;

		dynamic.Pos = dynamic.NextPos;
		dynamic.NextPos = dynamic.Pos + dynamic.Vector * singleStep;
	}

	return true;
}

CCollisionBlock*	CCollisionManager::GetBlock( const unsigned row, const unsigned col ) const
{
	if( row >= Rows || col >= Cols )
		return nullptr;

	return const_cast<CCollisionBlock*>(&Blocks[ static_cast<size_t>(Cols) * row + col ]);
}

const unsigned	CCollisionManager::FindBlockIndex( const glm::vec3& point ) const
{
	auto scl = point / glm::vec3( BlockWidth, 1.0f, BlockHeight );

	unsigned x = (unsigned)scl.x;
	unsigned y = (unsigned)scl.z;

	return y * Cols + x;
}

CCollisionBlock*	CCollisionManager::FindBlock( const glm::vec3& point ) const
{
	auto index = FindBlockIndex( point );
	if( index >= Blocks.size() )
		return nullptr;
	return const_cast<CCollisionBlock*>(&Blocks[index]);
}

const bool	CCollisionManager::FindCollisionForDynamic( const CDynamic& dynamic, CCollision& outCollision, const bool ignoreObjects ) const
{
	if( !dynamic.IsCollidable() )
		return false;

	std::vector<CCollision> collisions;
	//CCollisionBlock* pBlock = FindBlock( pDynamic->Pos );
	//if( pBlock != nullptr )
	//{
	//	FindFullBlockCollisions( *pBlock, *pDynamic, collisions );
	//}
	//for( unsigned i = 0; i < Blocks.size(); i++ )
	//{
	//	FindFullBlockCollisions( Blocks[i], *pDynamic, collisions );
	//}

	CCollisionBlock* pBlock = FindBlock( dynamic.Pos );
	if( pBlock != nullptr )
		FindFullBlockCollisions( *pBlock, dynamic, collisions );
	pBlock = FindBlock( dynamic.NextPos );
	if( pBlock != nullptr )
		FindFullBlockCollisions( *pBlock, dynamic, collisions );


	if( !ignoreObjects )
	{
		for( unsigned obj = 0; obj < ObjectList.size(); obj++ )
		{
			CObject* pObject = ObjectList[obj];

			if( !pObject->IsCollidable() )
				continue;

			auto toObjVec = pObject->Pos - dynamic.Pos;

			if( glm::dot(toObjVec, dynamic.Vector) <= 0.0f )
				continue;

			auto distFromDyn = glm::dot(dynamic.GetMoveVector(), toObjVec );
			auto distFromObj = glm::length(toObjVec) - distFromDyn;

			if(distFromObj >= dynamic.Radius + pObject->Radius)
				continue;

			auto reverse = dynamic.Radius + pObject->Radius - distFromObj;

			auto collisionDist = distFromDyn - reverse;

			float moveLen = glm::length(dynamic.GetMoveVector());
			if( collisionDist > moveLen )
				continue;

			auto collPoint = dynamic.Pos + dynamic.Vector * collisionDist - dynamic.Vector * dynamic.Radius;

			collisions.push_back( CCollision( collPoint, pObject ) );
		}
	}

	if( !collisions.empty() )
	{
		float dist = Distance( dynamic.Pos, collisions[0].Point );
		unsigned index = 0;
		for( unsigned i = 1; i < collisions.size(); i++ )
		{
			float newDist = Distance( dynamic.Pos, collisions[i].Point );
			if( newDist < dist )
			{
				index = i;
				dist = newDist;
			}
		}

		outCollision = collisions[index];

		return true;
	}

	return false;
}

void	CCollisionManager::FindBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions ) const
{
	for( unsigned i = 0; i < block.GetFaceNumber(); i++ )
	{
		auto& face = block.GetFace( i );

		glm::vec3 point;

		auto posCorrect = -face.Plane.Normal * dynamic.Radius;

		if( face.Plane.Intersects( dynamic.Pos, dynamic.NextPos + posCorrect, point ) ){
			if( face.CheckPointInFace( point ) )
				collisions.push_back( CCollision( point, face.Plane ) );
		}

		for( unsigned i = 0; i < 4; i++ )
		{
			auto pos = face.GetEdgeClosestPoint( i, dynamic.Pos );
			if( Distance( pos, dynamic.Pos ) > dynamic.Radius )
				continue;

			Planef colPlane( glm::normalize(dynamic.NextPos - pos), pos );
			posCorrect = -colPlane.Normal * dynamic.Radius;
			if( colPlane.Intersects( dynamic.Pos, dynamic.NextPos + posCorrect, point ) )
				collisions.push_back( CCollision( point, colPlane ) );
		}
	}
}

void	CCollisionManager::FindFullBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions ) const
{
	FindBlockCollisions( block, dynamic, collisions );

	for( unsigned i = 0; i < block.GetSideBlockNumber(); i++ )
		FindBlockCollisions( block.GetSideBlock( i ), dynamic, collisions );
}