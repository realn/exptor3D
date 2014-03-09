#include "CollisionFace.h"

CCollisionFace::CCollisionFace()
{

}

CCollisionFace::CCollisionFace( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 )
{
	Set( v1, v2, v3, v4 );
}

void	CCollisionFace::Set( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Vector3f& v4 )
{
	Vert[0] = v1;	Vert[1] = v2;	Vert[2] = v3;	Vert[3] = v4;
	Plane.Set( v2, v1, v3 );

	for( unsigned i = 0; i < 4; i++ )
	{
		Vector3f vec = Vert[ ( i + 1 ) % 4 ] - Vert[ i ]; 

		Edge[ i ].Normal = vec.Cross( Plane.Normal ).Normalize();
		Edge[ i ].D = -Edge[ i ].Normal.Dot( Vert[ i ] );
	}
}

const bool	CCollisionFace::CheckPointInFace( const Vector3f& point ) const
{
	for( unsigned i = 0; i < 4; i++ )
	{
		if( Edge[i].Distance( point ) < 0.0f )
			return false;
	}
	return true;
}