#pragma once

#include <glm/glm.hpp>
#include <vector>

class CLvlBlock;

class CLvlSector {
private:
	std::vector<CLvlBlock*>	m_Blocks;
	glm::mat4	m_Transform;

public:
	CLvlSector();
};