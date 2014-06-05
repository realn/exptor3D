#pragma once

#include <vector>
#include <glm/glm.hpp>

class CWireGrid {
private:
	std::vector<glm::vec3>	m_Verts;
	glm::vec4			m_Color;

public:
	CWireGrid( const glm::vec2& size, const glm::uvec2& slices, const glm::vec4& color );
	CWireGrid( const glm::vec2& size, const glm::uvec2& slices, const glm::vec4& color, const glm::vec3& origin );
	~CWireGrid();

	void	Render();

private:
	void	Create( const glm::vec2& size, const glm::uvec2& slices, const glm::vec3& origin );
};