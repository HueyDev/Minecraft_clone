#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <game.hpp>
#include <iostream>
#include <chrono>

//#define REPLIT

bool firstMouse = true;
float lastY, lastX;

GLFWwindow* window;

auto lastFrame = std::chrono::high_resolution_clock::now();
double lastTime = 0;
int numOfFrames = 0;

glm::vec3 cameraDir = glm::vec3(0, 0, 0);

Game *game;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			cameraDir += glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_S:
			cameraDir -= glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_D:
			cameraDir -= glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_A:
			cameraDir += glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_SPACE:
			cameraDir += glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			cameraDir -= glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W:
			cameraDir -= glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_S:
			cameraDir += glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_D:
			cameraDir += glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_A:
			cameraDir -= glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_SPACE:
			cameraDir -= glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			cameraDir += glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "MOUSE CALLBACK";
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  // std::cout << "Mouse movement: " << xoffset << " , " << yoffset << std::endl;
  // std::cout << "Last pos: " << lastX << " , " << lastY << std::endl;
  // std::cout << "Current pos: " << xpos << " , " << ypos << std::endl;

	//glfwSetCursorPos(window, 0, 0);

	lastX = xpos;
	lastY = ypos;

	game->camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  #ifndef REPLIT
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  #endif

}



void render() {
	numOfFrames++;
  if(glfwGetTime()-lastTime >= 1){
    std::cout << numOfFrames << std::endl;
    numOfFrames = 0;
    lastTime = glfwGetTime();
  }

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	game->render();
}

void update(float deltaTime) {
	
	game->camera->ProcessKeyboard(cameraDir, deltaTime);

}



int main() {
	
#pragma region INIT_GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

  #ifndef REPLIT
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  #endif

  //glfwWindowHint(GLFW_SAMPLES, 16);
  //glEnable(GL_MULTISAMPLE);

#pragma endregion

	glfwSwapInterval(1);

	game = new Game();

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();


		float deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFrame).count();
		lastFrame = std::chrono::high_resolution_clock::now();

		update(deltaTime);

	}


}

