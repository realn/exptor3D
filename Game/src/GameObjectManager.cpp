#include "GameObjectManager.h"

CObjectManager::CObjectManager()
{
}

const bool	CObjectManager::Contains(CObject* pObject) const
{
	if( std::find(m_objectList.cbegin(), m_objectList.cend(), pObject) != m_objectList.cend())
		return true;

	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
	{
		if( std::find(m_dynamicList.cbegin(), m_dynamicList.cend(), pDynamic) != m_dynamicList.cend())
			return true;
	}
	return false;
}

void	CObjectManager::AddObject(CObject* pObject)
{
	if( Contains(pObject) )
		return;

	IEntity* pEntity = dynamic_cast<IEntity*>(pObject);
	if(pEntity != 0)
		m_entityList.push_back(pEntity);

	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
		m_dynamicList.push_back(pDynamic);
	else
		m_objectList.push_back(pObject);
}

void	CObjectManager::DeleteObject(CObject* pObject)
{
	IEntity* pEntity = dynamic_cast<IEntity*>(pObject);
	if(pEntity != 0)
	{
		auto index = std::find(m_entityList.begin(), m_entityList.end(), pEntity);
		if(index != m_entityList.end())
			m_entityList.erase(index);
	}

	CDynamic* pDynamic = dynamic_cast<CDynamic*>(pObject);
	if(pDynamic != 0)
	{
		auto index = std::find(m_dynamicList.begin(), m_dynamicList.end(), pDynamic);
		if(index != m_dynamicList.end())
			m_dynamicList.erase(index);
	}
	else
	{
		auto index = std::find(m_objectList.begin(), m_objectList.end(), pObject);
		if(index != m_objectList.end())
			m_objectList.erase(index);
	}

	delete pObject;
}

void	CObjectManager::Clear()
{
	for(unsigned i = 0; i < m_objectList.size(); i++)
		delete m_objectList[i];
	for(unsigned i = 0; i < m_dynamicList.size(); i++)
		delete m_dynamicList[i];

	m_entityList.clear();
	m_dynamicList.clear();
	m_objectList.clear();
}

void	CObjectManager::Update( const float fTD )
{
	std::vector<IEntity*> entityList(m_entityList);
	for(unsigned i = 0; i < entityList.size(); i++)
		entityList[i]->Update( fTD );
}