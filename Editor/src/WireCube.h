#pragma once

#include <glm/glm.hpp>
#include <vector>

class CWireCube {
private:
	std::vector<glm::vec3>	m_Verts;
	glm::vec4	m_Color;

public:
	CWireCube( const glm::vec3& size, const glm::vec4& color );
	CWireCube( const glm::vec3& size, const glm::vec4& color, const glm::vec3& origin );
	~CWireCube();

	void	Render();

private:
	void	Create( const glm::vec3& size, const glm::vec3& origin );
};