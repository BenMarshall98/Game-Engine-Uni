#pragma once
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>

//TODO: Make the input generic
class GLFWInput
{
private:
	GLFWwindow * gameWindow;
public:
	GLFWInput(GLFWwindow * pGameWindow) : gameWindow(pGameWindow) {}

	float KeyboardInput(int key);
	float MouseInput(int key);
	float GamePadInput(int key);

	~GLFWInput();
};

