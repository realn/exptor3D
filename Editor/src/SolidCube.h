#pragma once

#include <glm/glm.hpp>
#include <vector>

class CSolidCube {
private:
	std::vector<glm::vec3>	m_Verts;
	std::vector<unsigned short>	m_Indices;
	glm::vec4	m_Color;

public:
	CSolidCube(const glm::vec3& size, const glm::vec4& color);
	CSolidCube(const glm::vec3& size, const glm::vec4& color, const glm::vec3& origin);
	~CSolidCube();

	void	Render();
	void	Render( const glm::vec4& color );

private:
	void	Create( const glm::vec3& size, const glm::vec3& origin );
	void	AddQuad( const unsigned v0, const unsigned v1, const unsigned v2, const unsigned v3 );
	void	AddTriangle( const unsigned v0, const unsigned v1, const unsigned v2 );
};