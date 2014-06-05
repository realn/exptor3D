#pragma once

#include <glm/glm.hpp>

#include "Plane.h"

class CSurface {
private:
	CPlane		m_SurfacePlane;
	CPlane		m_EdgePlane[4];
	glm::vec3	m_Verts[4];

public:
	CSurface();
	CSurface( const glm::vec3* const verts );

	const CPlane&		GetPlane() const;
	const CPlane&		GetEdgePlane( const unsigned edge ) const;
	const glm::vec3&	GetVert( const unsigned vert ) const;
	
	void	Set( const glm::vec3* const verts );
	void	Set( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 );

	const bool	IsEqual( const CSurface& surface, const bool sameSide = true ) const;
};