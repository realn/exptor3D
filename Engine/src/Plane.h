#pragma once

#include <glm/glm.hpp>

class CPlane {
public:
	glm::vec3	N;
	float		D;

	CPlane();
	CPlane( const glm::vec3& normal, const float dist );
	CPlane( const glm::vec3& normal, const glm::vec3& point );
	CPlane( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 );

	const float	Distance( const glm::vec3& point ) const;
	const bool	Intersects( const glm::vec3& rayOrigin, const glm::vec3& rayVector ) const;
	const bool	Intersects( const glm::vec3& rayOrigin, const glm::vec3& rayVector, glm::vec3& outPoint ) const;

	void	Set( const glm::vec3& normal, const glm::vec3& point );
	void	Set( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 );

	const bool	IsEqual( const CPlane& plane, const bool sameSide = true ) const;
};

extern const glm::vec3	MakeNormal( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 );