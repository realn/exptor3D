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
	
extern const glm::vec3	GetBlockPosWS( const glm::ivec3& pos, const glm::vec3& blockSize = glm::vec3(1.0f) );
extern const glm::ivec3	GetBlockPosBS( const glm::vec3& pos, const glm::vec3& blockSize = glm::vec3(1.0f) );
extern const glm::vec3	ClampBlockPosWS( const glm::vec3& pos, const glm::vec3& blockSize = glm::vec3(1.0f) );