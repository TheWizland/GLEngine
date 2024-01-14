#include "InputHandler.h"
#include <GLFW\glfw3.h>

InputHandler::InputHandler(std::function<void(float)> func)
{
	action = func;
}

void InputHandler::handleInput(float deltaTime)
{
	if (isActive)
	{
		action(deltaTime);
		if (!repeat)
			isActive = false;
	}
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::map<int,InputHandler*>::iterator iter = inputMap.find(key);
	if (iter == inputMap.end()) {
		return;
	}
	if (action == GLFW_PRESS) {
		iter->second->activate();
	}
	if (action == GLFW_RELEASE) {
		iter->second->deactivate();
	}
}

void InputManager::checkInputs(float deltaTime)
{
	for (InputHandler* input : inputList)
	{
		input->handleInput(deltaTime);
	}
}

void InputManager::addAction(int key, std::function<void(float)> func)
{
	InputHandler* inputHandler = new InputHandler(func);
	inputMap.insert(std::pair<int, InputHandler*>(key, inputHandler));
	inputList.push_back(inputHandler);
}