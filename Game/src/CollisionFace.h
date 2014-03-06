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

	void	Set( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 );
};