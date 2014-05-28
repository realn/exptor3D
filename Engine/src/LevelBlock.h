#pragma once

#include <glm/glm.hpp>
#include <vector>

class CLvlSurface;

class CLvlBlock {
private:
	std::vector<CLvlSurface*>	m_Surfaces;
	glm::mat4	m_Transform;

public:
	CLvlBlock();
};