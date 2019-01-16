#include "GLFWInput.h"
#include <iostream>
using namespace std;

GLFWInput::~GLFWInput()
{
}

float GLFWInput::KeyboardInput(int key)
{
	if (!keyboardAllowed)
	{
		return 0;
	}

	int state = glfwGetKey(gameWindow, key);

	if (state == GLFW_PRESS)
	{
		return 1;
	}
	return 0;
}

float GLFWInput::MouseInput(int key)
{
	if (!mouseAllowed)
	{
		return 0;
	}

	if (key < 4)
	{
		double XPos, YPos;
		glfwGetCursorPos(gameWindow, &XPos, &YPos);
		if (YPos < 0 || YPos > GLFWWindow::GetHeight() || XPos < 0 || XPos > GLFWWindow::GetWidth())
		{
			return 0;
		}

		if (key == 0 && YPos < GLFWWindow::GetHeight() / 2)
		{
			return (float)((GLFWWindow::GetHeight() / 2) - YPos) / (GLFWWindow::GetHeight() / 2);
		}
		else if (key == 1 && YPos > GLFWWindow::GetHeight() / 2)
		{
			return (float)(YPos - (GLFWWindow::GetHeight() / 2)) / (GLFWWindow::GetHeight() / 2);
		}
		else if (key == 2 && XPos < GLFWWindow::GetWidth() / 2)
		{
			return (float)((GLFWWindow::GetWidth() / 2) - XPos) / (GLFWWindow::GetWidth() / 2);
		}
		else if (key == 3 && XPos > GLFWWindow::GetWidth() / 2)
		{
			return (float)(XPos - (GLFWWindow::GetWidth() / 2)) / (GLFWWindow::GetWidth() / 2);
		}
	}
	else
	{
		int state = glfwGetMouseButton(gameWindow, key - 4);

		if (state == GLFW_PRESS)
		{
			return 1;
		}
		return 0;
	}
}

float GLFWInput::GamePadInput(int key) const
{
	if (!gamepadAllowed)
	{
		return 0;
	}

	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (present == GLFW_FALSE)
	{
		return 0;
	}

	int count;
	
	if (key < 14)
	{
		const unsigned char * axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

		if (key >= count)
		{
			return 0;
		}
		if (axes[key] == GLFW_PRESS)
		{
			return 1;
		}
		return 0;
	}

	int keySlot = (key - 14) / 2;

	const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

	if (keySlot >= count)
	{
		return 0;
	}

	if (axes[keySlot] > 0 && (key % 2) != 0)
	{
		return axes[keySlot];
	}
	else if (axes[keySlot] < 0 && (key % 2) == 0)
	{
		return -axes[keySlot];
	}
	return 0;
}


void GLFWInput::AllowMouseInput(bool allowMouse)
{
	mouseAllowed = allowMouse;
}

void GLFWInput::AllowKeyboardInput(bool allowKeyboard)
{
	keyboardAllowed = allowKeyboard;
}

void GLFWInput::AllowGamePadInput(bool allowGamePad)
{
	gamepadAllowed = allowGamePad;
}