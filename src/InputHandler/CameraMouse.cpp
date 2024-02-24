#include "CameraMouse.h"

void CameraMouse::checkMouse(GLFWwindow* window) 
{
	if (!enabled)
		return;

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	double centerX = windowWidth / 2.0;
	double centerY = windowHeight / 2.0;

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	double deltaX = mouseX - centerX;
	double deltaY = centerY - mouseY;

	camera->globalYaw((float)(deltaX*sensitivityX));
	camera->pitch((float)(deltaY*sensitivityY));

	
	glfwSetCursorPos(window, centerX, centerY);
}

void CameraMouse::init(GLFWwindow* window, Camera* camera)
{
	this->camera = camera;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	double centerX = windowWidth / 2.0;
	double centerY = windowHeight / 2.0;
	glfwSetCursorPos(window, centerX, centerY);

	enabled = true;
}

void CameraMouse::setSensitivity(double sensitivity)
{
	this->sensitivityX = sensitivity;
	this->sensitivityY = sensitivity;
}

void CameraMouse::setSensitivity(double sensitivityX, double sensitivityY)
{
	this->sensitivityX = sensitivityX;
	this->sensitivityY = sensitivityY;
}
