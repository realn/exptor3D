#include "CollisionManager.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
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
		for( unsigned obj = 0; obj < ObjectList.size(); obj++ )
		{
			CObject* pObject = ObjectList[obj];

			auto toObjVec = pObject->Pos - pDynamic->Pos;

			if( toObjVec.Dot( pDynamic->Vector ) <= 0.0f )
				continue;

			auto distFromDyn = pDynamic->GetMoveVector().Dot( toObjVec );
			auto distFromObjSq = POW( toObjVec.Leangth() ) - POW ( distFromDyn );

			if( distFromObjSq >= POW( pDynamic->Radius + pObject->Radius ) )
				continue;

			auto reverseSq = POW( pDynamic->Radius + pObject->Radius ) - distFromObjSq;
			auto reverse = sqrtf( reverseSq );

			auto collisionDist = distFromDyn - reverse;

			if( collisionDist > pDynamic->GetMoveVector().Leangth() )
				continue;

			if( pDynamic->OnCollision( pObject ) )
			{
				pDynamic->NextPos = pDynamic->Pos + pDynamic->Vector * collisionDist;
			}
		}
	}
}