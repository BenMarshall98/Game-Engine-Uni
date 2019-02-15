#include "GLFWInput.h"
#include <iostream>
using namespace std;

GLFWInput::~GLFWInput()
{
}

float GLFWInput::KeyboardInput(const int key) const
{
	if (!keyboardAllowed)
	{
		return 0;
	}

	const int state = glfwGetKey(gameWindow, key);

	if (state == GLFW_PRESS)
	{
		return 1;
	}
	return 0;
}

float GLFWInput::MouseInput(const int key) const 
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

		//TODO: Look at if the get height / 2 is actually working

		if (key == 0 && YPos < GLFWWindow::GetHeight() / 2)
		{
			return static_cast<float>((GLFWWindow::GetHeight() / 2) - YPos) / (GLFWWindow::GetHeight() / 2);
		}
		else if (key == 1 && YPos > GLFWWindow::GetHeight() / 2)
		{
			return static_cast<float>(YPos - (GLFWWindow::GetHeight() / 2)) / (GLFWWindow::GetHeight() / 2);
		}
		else if (key == 2 && XPos < GLFWWindow::GetWidth() / 2)
		{
			return static_cast<float>((GLFWWindow::GetWidth() / 2) - XPos) / (GLFWWindow::GetWidth() / 2);
		}
		else if (key == 3 && XPos > GLFWWindow::GetWidth() / 2)
		{
			return static_cast<float>(XPos - (GLFWWindow::GetWidth() / 2)) / (GLFWWindow::GetWidth() / 2);
		}
	}
	else
	{
		const int state = glfwGetMouseButton(gameWindow, key - 4);

		if (state == GLFW_PRESS)
		{
			return 1;
		}
		return 0;
	}
}

float GLFWInput::GamePadInput(const int key) const
{
	if (!gamepadAllowed)
	{
		return 0;
	}

	const int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (present == GLFW_FALSE)
	{
		return 0;
	}

	int count;
	
	if (key < 14)
	{
		const unsigned char * const axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

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

	const int keySlot = (key - 14) / 2;

	const float * const axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

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


void GLFWInput::AllowMouseInput(const bool allowMouse)
{
	mouseAllowed = allowMouse;
}

void GLFWInput::AllowKeyboardInput(const bool allowKeyboard)
{
	keyboardAllowed = allowKeyboard;
}

void GLFWInput::AllowGamePadInput(const bool allowGamePad)
{
	gamepadAllowed = allowGamePad;
}