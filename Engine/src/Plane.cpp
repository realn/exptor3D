#include "stdafx.h"
#include "Plane.h"

CPlane::CPlane() :
	N( 1.0f, 0.0f, 0.0f ), D(0.0f)
{
}

CPlane::CPlane( const glm::vec3& normal, const float dist ) :
	N( normal ), D( dist )
{
}

CPlane::CPlane( const glm::vec3& normal, const glm::vec3& point ) {
	this->Set( normal, point );
}

CPlane::CPlane( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ) {
	this->Set( v0, v1, v2 );
}

const float	CPlane::Distance( const glm::vec3& point ) const {
	return glm::dot( N, point ) + D;
}

const bool	CPlane::Intersects( const glm::vec3& rayOrigin, const glm::vec3&  rayVector ) const {
	glm::vec3 result;
	return this->Intersects( rayOrigin, rayVector, result );
}

const bool	CPlane::Intersects( const glm::vec3& rayOrigin, const glm::vec3&  rayVector, glm::vec3& outPoint ) const {
	float counter = Distance( rayOrigin );
	float w = counter * Distance( rayOrigin + rayVector );
	if( w >= 0.0f )
		return false;

	float denom = -glm::dot(N, rayVector );

	outPoint = rayOrigin + rayVector * ( counter / denom );
	return true;
}

void	CPlane::Set( const glm::vec3& normal, const glm::vec3& point ) {
	this->N = normal;
	this->D = -glm::dot( normal , point );
}

void	CPlane::Set( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ) {
	this->Set( MakeNormal( v0, v1, v2 ), v0 );
}

const bool	CPlane::IsEqual( const CPlane& plane, const bool sameSide ) const {
	if( !sameSide && this->N == -plane.N && this->D == -plane.D )
		return true;
	return this->N == plane.N && this->D == plane.D;
}

const bool	CPlane::operator==( const CPlane& plane ) const {
	return IsEqual( plane, true );
}

const bool	CPlane::operator!=( const CPlane& plane ) const {
	return !IsEqual( plane, true );
}

const bool	CPlane::operator<( const CPlane& plane ) const {
	return VectorLess( this->N, plane.N ) && this->D < plane.D;
}

const glm::vec3	MakeNormal( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ) {
	const glm::vec3 vec1 = v1 - v0;
	const glm::vec3 vec2 = v2 - v0;

	return glm::normalize( glm::cross( vec1, vec2 ) );
}

const bool	VectorLess( const glm::vec3& v0, const glm::vec3& v1 ) {
	return v0.x < v1.x && v0.y < v1.y && v0.z < v1.z;
}