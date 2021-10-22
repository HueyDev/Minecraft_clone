#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


namespace vep {

	glm::mat4 getViewMatrix();

	glm::vec3 getPos();

	void ProcessKeyboard(glm::vec3 dir, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch);

}

