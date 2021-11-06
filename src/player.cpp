#include <player.hpp>
#include <iostream>



namespace vep {

	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 worldUp = up;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float speed = 10.0f;
	float MouseSensitivity = 1.0f;
	float ZOOM = 45.0f;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

	glm::mat4 getViewMatrix()
	{
		/*std::cout << glm::to_string(position) << std::endl;
		std::cout << glm::to_string(front) << std::endl;
		std::cout << glm::to_string(up) << std::endl;
		std::cout << glm::to_string(worldUp) << std::endl;
		std::cout << glm::to_string(glm::lookAt(position, position + front, up)) << "\n"; */
		return glm::lookAt(position, position + front, up);
	}

	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new front vector
		glm::vec3 lfront;
		lfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		lfront.y = sin(glm::radians(pitch));
		//front.y = 0;
		lfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(lfront);
		// also re-calculate the right and up vector
		right = glm::normalize(glm::cross(lfront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, lfront));
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainpitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}


        //std::cout << "Mouse info " << pitch << " , " << yaw << "\n";
		//std::cout << this->pitch << std::endl;

		// update front, right and up Vectors using the updated Euler angles
		updateCameraVectors();
		//std::cout << glm::to_string(front) << std::endl;
	}

	void ProcessKeyboard(glm::vec3 dir, float deltaTime)
	{
		float velocity = speed * deltaTime;
		if (dir.z == 1)
			position += glm::normalize(glm::vec3(front.x, 0, front.z)) * velocity;
		if (dir.z == -1)
			position -= glm::normalize(glm::vec3(front.x, 0, front.z)) * velocity;
		if (dir.x == 1)
			position -= right * velocity;
		if (dir.x == -1)
			position += right * velocity;
		if (dir.y == 1)
			position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (dir.y == -1)
			position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
	}

	glm::vec3 getPos() {
		return position;
	}
}