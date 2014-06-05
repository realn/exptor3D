#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class PLANE_ORIENTATION {
	AXIS_XY,
	AXIS_XZ,
	AXIS_YZ,
};

class CGraphicPlane {
private:
	std::vector<glm::vec3>	m_Verts;
	glm::vec4	m_Color;

public:
	CGraphicPlane( const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec4& color );
	CGraphicPlane( const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec4& color, const glm::vec3& origin );
	~CGraphicPlane();

	void	Render();
	
private:
	void	Create(const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec3& origin );
	const glm::vec3	MakeVertex( const PLANE_ORIENTATION ori, const glm::vec3& start, const glm::vec2& size, const unsigned x, const unsigned y );
};