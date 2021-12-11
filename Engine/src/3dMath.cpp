/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	3dMath.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> 3dMath.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <glm/glm.hpp>

#include "3dMath.h"

Planef::Planef() = default;

Planef::Planef( const glm::vec3& normal, const float distance ) :
	Normal( normal ),
	D( distance )
{}

Planef::Planef( const glm::vec3& normal, const glm::vec3& pos ) {
	Set( normal, pos );
}

Planef::Planef( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 ) {
	Set( v1, v2, v3 );
}

void	Planef::Set( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 ) {
	Normal = MakeNormal( v1, v2, v3 );
	D = -glm::dot(Normal, v1);
}

void	Planef::Set( const glm::vec3& normal, const glm::vec3& pos ) {
	Normal = normal;
	D = -glm::dot(Normal, pos);
}

const float	Planef::Distance(const glm::vec3& pos) const {
	return glm::dot(Normal, pos) + D;
}

const bool	Planef::Intersects( const glm::vec3& origin, const glm::vec3& dest, glm::vec3& outIntersect ) const {
	float w = Distance( origin ) * Distance( dest );
	if( w >= 0.0f )
		return false;

	auto vec = dest - origin;

	auto counter = Distance( origin );
	auto denom = -glm::dot(Normal, vec);

	outIntersect = origin + vec * ( counter / denom );
	return true;
}


glm::vec3	MakeNormal( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 ) {
	auto vec1 = v2 - v1;
	auto vec2 = v3 - v1;
	return glm::normalize(glm::cross(vec1, vec2));
}

glm::vec3	MakeVectorXZ( const float degAngle ) {
	auto angle = glm::radians(degAngle);
	return glm::vec3(glm::sin(angle), 0.0f, glm::cos(angle));
}

/*	DYSTANS
	Ta funkcja oblicza odleg³oœæ
	miêdzy dwoma punktami.
*/
float Distance( const glm::vec3& V1, const glm::vec3& V2 )
{
	auto vect = V2 - V1;
	return glm::length(vect);
}

float	pythagoreanTheorem( const float a, const float b )
{
	return glm::sqrt(glm::pow(a, 2.0f) + glm::pow(b, 2.0f));
}

glm::vec3 ClosestPoint( const glm::vec3 &V1, const glm::vec3 &V2, const glm::vec3 &Point )
{
	auto Vect1 = Point - V1;
	auto Vect2 = glm::normalize(V2 - V1);

	float d = Distance( V1, V2 );
	float t = glm::dot(Vect2, Vect1);

	if( t <= 0 )
		return V1;

	if( t >= d )
		return V2;

	auto result = V1 + Vect2 * t;

	return result;
}

float GetAngle( const glm::vec3& V1, const glm::vec3 &V2 )
{
	auto vec1 = V2 - V1;
	auto origin = glm::vec3( 0.0f, 0.0f, 1.0f );

	float angRad = std::atan2f( vec1.z, vec1.x ) - std::atan2f( origin.z, origin.x );

	return glm::degrees(angRad);
}

float SwapAngle( float degAngle ) {
	while(degAngle > 180.0f )
		degAngle -= 360.0f;
	while(degAngle < -180.0f )
		degAngle += 360.0f;

	return degAngle;
}
