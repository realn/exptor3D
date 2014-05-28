#pragma once

#include <glm/glm.hpp>

class CLvlSurface {
private:
	glm::vec3	m_Verts[4];
	glm::vec2	m_Cords[4];
	glm::vec3	m_Normal;
	int			m_MaterialID;

	bool	m_Active;
	bool	m_Visible;
	
public:
	CLvlSurface();
};