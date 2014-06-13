#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class CGenericMesh {
private:
	std::vector<glm::vec3>	m_Verts;
	std::vector<unsigned short>	m_Indices;

public:
	CGenericMesh();
	~CGenericMesh();

	void	Render( GLenum primitiveMode );

	void	Clear();
	void	AddPoint( const glm::vec3& v0 );
	void	AddLine( const glm::vec3& v0, const glm::vec3& v1 );
	void	AddTriangle( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 );
	void	AddTriQuad( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 );

private:
	const unsigned short	InsertVertex( const glm::vec3& vertex );
};