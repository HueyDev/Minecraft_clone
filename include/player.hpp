#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <iostream>


namespace vep {

	glm::mat4 getViewMatrix();

	glm::vec3 getPos();

	void ProcessKeyboard(glm::vec3 dir, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch);

	void updateCameraVectors();

	void setPos(glm::vec3 p);
}

