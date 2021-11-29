#include <glm/glm.hpp>

#include "CollisionFace.h"

CCollisionFace::CCollisionFace()
{

}

CCollisionFace::CCollisionFace( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 )
{
	Set( v1, v2, v3, v4 );
}

void	CCollisionFace::Set( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 )
{
	Vert[0] = v1;	Vert[1] = v2;	Vert[2] = v3;	Vert[3] = v4;
	Plane.Set( v2, v1, v3 );

	for( unsigned i = 0; i < 4; i++ )
	{
		glm::vec3 vec = Vert[ ( i + 1 ) % 4 ] - Vert[ i ]; 

		Edge[i].Normal = glm::normalize(glm::cross(vec, Plane.Normal));
		Edge[ i ].D = -glm::dot(Edge[ i ].Normal, Vert[ i ] );
	}
}

const bool	CCollisionFace::CheckPointInFace( const glm::vec3& point ) const
{
	for( unsigned i = 0; i < 4; i++ )
	{
		if( Edge[i].Distance( point ) < 0.0f )
			return false;
	}
	return true;
}

const glm::vec3	CCollisionFace::GetEdgeClosestPoint( const unsigned edge, const glm::vec3& pos ) const
{
	auto& v1 = Vert[edge % 4];
	auto& v2 = Vert[ ( edge + 1 ) % 4 ];

	return ClosestPoint( v1, v2, pos );
}