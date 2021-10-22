#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <global.hpp>

namespace ve {
	VoxelType* getChunk(glm::vec2 pos);
	glm::vec2 worldToChunkLoc(glm::vec3 pos);
}