#include "CollisionManager.h"
#include "Log.h"

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

		if( !pDynamic->IsCollidable() )
			continue;

		std::vector<CCollision> collisions;
		//CCollisionBlock* pBlock = FindBlock( pDynamic->Pos );
		//if( pBlock != nullptr )
		//{
		for( unsigned i = 0; i < Blocks.size(); i++ )
		{
			Planef plane; Vector3f point;
			if( FindFullBlockCollisions( Blocks[i], *pDynamic, collisions ) )
			{
				if( pDynamic->OnCollision( point, plane ) )
				{
					pDynamic->NextPos = point + plane.Normal * pDynamic->Radius;
				}

				continue;
			}
		}
		//}

		if( !collisions.empty() )
		{
			float dist = DistanceSq( pDynamic->NextPos, collisions[0].Point );
			unsigned index = 0;
			for( unsigned i = 1; i < collisions.size(); i++ )
			{
				float newDist = DistanceSq( pDynamic->NextPos, collisions[i].Point );
				if( newDist < dist )
				{
					index = i;
					dist = newDist;
				}
			}

			auto& closest = collisions[index];
			if( pDynamic->OnCollision( closest.Point, closest.Plane ) )
				pDynamic->NextPos = closest.Point + closest.Plane.Normal * pDynamic->Radius;
		}


		for( unsigned obj = 0; obj < ObjectList.size(); obj++ )
		{
			CObject* pObject = ObjectList[obj];

			if( !pObject->IsCollidable() )
				continue;

			auto toObjVec = pObject->Pos - pDynamic->Pos;

			if( toObjVec.Dot( pDynamic->Vector ) <= 0.0f )
				continue;

			auto distFromDyn = pDynamic->GetMoveVector().Dot( toObjVec );
			auto distFromObjSq = POW( toObjVec.Length() ) - POW ( distFromDyn );

			if( distFromObjSq >= POW( pDynamic->Radius + pObject->Radius ) )
				continue;

			auto reverseSq = POW( pDynamic->Radius + pObject->Radius ) - distFromObjSq;
			auto reverse = sqrtf( reverseSq );

			auto collisionDist = distFromDyn - reverse;

			if( collisionDist > pDynamic->GetMoveVector().Length() )
				continue;

			if( pDynamic->OnCollision( pObject ) )
			{
				pDynamic->NextPos = pDynamic->Pos + pDynamic->Vector * collisionDist;
			}
		}
	}
}

CCollisionBlock*	CCollisionManager::GetBlock( const unsigned row, const unsigned col )
{
	if( row >= Rows || col >= Cols )
		return nullptr;
	
	return &Blocks[ Cols * row + col ];
}

const unsigned	CCollisionManager::FindBlockIndex( const Vector3f& point )
{
	auto scl = point / Vector3f( BlockWidth, 1.0f, -BlockHeight );

	unsigned x = (unsigned)scl.X;
	unsigned y = (unsigned)scl.Z;

	return y * Cols + x;
}

CCollisionBlock*	CCollisionManager::FindBlock( const Vector3f& point )
{
	auto index = FindBlockIndex( point );
	if( index >= Blocks.size() )
		return nullptr;
	return &Blocks[index];
}

const bool	CCollisionManager::FindBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions )
{
	for( unsigned i = 0; i < block.GetFaceNumber(); i++ )
	{
		auto& face = block.GetFace( i );

		Vector3f point;

		auto posCorrect = -face.Plane.Normal * dynamic.Radius;

		if( face.Plane.Intersects( dynamic.Pos, dynamic.NextPos + posCorrect, point ) ){
			if( !face.CheckPointInFace( point ) )
				continue;

			collisions.push_back( CCollision( point, face.Plane ) );
		}
	}

	return false;
}

const bool	CCollisionManager::FindFullBlockCollisions( const CCollisionBlock& block, const CDynamic& dynamic, std::vector<CCollision>& collisions )
{
	if( FindBlockCollisions( block, dynamic, collisions ) )
		return true;

	for( unsigned i = 0; i < block.GetSideBlockNumber(); i++ )
	{
		if( FindBlockCollisions( block.GetSideBlock( i ), dynamic, collisions ) )
			return true;
	}

	return false;
}