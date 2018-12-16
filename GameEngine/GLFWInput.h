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
	explicit GLFWInput(GLFWwindow * pGameWindow) : gameWindow(pGameWindow) {}

	float KeyboardInput(int key);
	float MouseInput(int key);
	float GamePadInput(int key) const;

	void AllowMouseInput(bool allowMouse);
	void AllowKeyboardInput(bool allowKeyboard);
	void AllowGamePadInput(bool allowGamePad);

	~GLFWInput();
};

