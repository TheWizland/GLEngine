#pragma once
#include <GLFW/glfw3.h>
#include "../Camera.h"

class CameraMouse
{
private:
	double sensitivityX = 0.01;
	double sensitivityY = 0.01;
	bool isRecentering = false;
	bool enabled = false;
	Camera* camera;
	GLFWwindow* window;
public:
	void checkMouse(GLFWwindow* window);
	void init(GLFWwindow* window, Camera* camera);
	void setSensitivity(double sensitivity);
	void setSensitivity(double sensitivityX, double sensitivityY);
};