#pragma once
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>

//TODO: Make the input generic
class GLFWInput
{
private:
	GLFWwindow * gameWindow;
	bool mouseAllowed = true;
	bool keyboardAllowed = true;
	bool gamepadAllowed = true;
public:
	explicit GLFWInput(GLFWwindow * const pGameWindow) : gameWindow(pGameWindow) {}

	GLFWInput& operator= (const GLFWInput&) = delete;
	GLFWInput(GLFWInput&) = delete;

	float KeyboardInput(int key) const;
	float MouseInput(int key) const;
	float GamePadInput(int key) const;

	void AllowMouseInput(bool allowMouse);
	void AllowKeyboardInput(bool allowKeyboard);
	void AllowGamePadInput(bool allowGamePad);

	~GLFWInput();
};

