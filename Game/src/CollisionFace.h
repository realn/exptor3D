#pragma once

#include "3dMath.h"

class CCollisionFace
{
public:
	Vector3f	Vert[4];
	Planef		Plane;
	Planef		Edge[4];

public:
	CCollisionFace();
	CCollisionFace( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 );

	void	Set( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 );

	const bool	CheckPointInFace( const Vector3f& point ) const;
};