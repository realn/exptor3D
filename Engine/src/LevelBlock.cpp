#include "LevelBlock.h"
#include "stdafx.h"

const glm::vec3	GetBlockPosWS( const glm::ivec3& pos, const glm::vec3& blockSize ) {
	return glm::vec3(pos) * blockSize;
}

const glm::ivec3	GetBlockPosBS( const glm::vec3& pos, const glm::vec3& blockSize ) {
	return glm::ivec3(glm::round(pos / blockSize));
}

const glm::vec3	ClampBlockPosWS( const glm::vec3& pos, const glm::vec3& blockSize ) {
	return GetBlockPosWS( GetBlockPosBS( pos, blockSize ), blockSize );
}