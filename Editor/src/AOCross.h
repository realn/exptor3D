#pragma once

#include <glm/glm.hpp>

class CAOCross {
private:
	glm::vec3	m_Verts[6];
	glm::vec4	m_Colors[6];

public:
	CAOCross();
	~CAOCross();

	void	Render();
};