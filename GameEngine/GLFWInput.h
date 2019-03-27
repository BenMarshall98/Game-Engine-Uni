#pragma once
#include "GLFWWindow.h"
#include "Input.h"
#include <GLFW/glfw3.h>

class GLFWInput : public Input
{
private:
	GLFWwindow * gameWindow;
public:
	explicit GLFWInput(GLFWwindow * const pGameWindow) : gameWindow(pGameWindow) {}

	GLFWInput& operator= (const GLFWInput&) = delete;
	GLFWInput(GLFWInput&) = delete;

	float KeyboardInput(int key) const override;
	float MouseInput(int key) const override;
	float GamePadInput(int key) const override;

	~GLFWInput();
};

