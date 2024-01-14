#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <functional>

class InputHandler
{
private:
	bool isActive = false;
	bool repeat = true;
	std::function<void(float)> action;
public:
	InputHandler(std::function<void(float)> func);
	void activate() { isActive = true; }
	void deactivate() { isActive = false; }
	void handleInput(float deltaTime);
	void runAction(float deltaTime) {
		action(deltaTime);
	}
};

class InputManager 
{
private:
	std::map<int, InputHandler*> inputMap;
	std::vector<InputHandler*> inputList;
public:
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void checkInputs(float deltaTime);
	void addAction(int key, std::function<void(float)> func);
};


