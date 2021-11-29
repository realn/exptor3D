#pragma once

#include "3dMath.h"

class CCollisionFace
{
public:
	glm::vec3	Vert[4];
	Planef		Plane;
	Planef		Edge[4];

public:
	CCollisionFace();
	CCollisionFace( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 );

	void	Set( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4 );

	const bool		CheckPointInFace( const glm::vec3& point ) const;
	const glm::vec3	GetEdgeClosestPoint( const unsigned edge, const glm::vec3& pos ) const;
};