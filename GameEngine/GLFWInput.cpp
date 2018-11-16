#include "GLFWInput.h"

GLFWInput::~GLFWInput()
{
}

float GLFWInput::KeyboardInput(int key)
{
	int state = glfwGetKey(gameWindow, key);

	if (state == GLFW_PRESS)
	{
		return 1;
	}
	return 0;
}

float GLFWInput::MouseInput(int key)
{
	if (key < 4)
	{
		double XPos, YPos;
		glfwGetCursorPos(gameWindow, &XPos, &YPos);

		if (key == 0 && YPos < GLFWWindow::height / 2)
		{
			return YPos / (GLFWWindow::height / 2);
		}
		else if (key == 1 && YPos > GLFWWindow::height / 2)
		{
			return (YPos - (GLFWWindow::height / 2)) / (GLFWWindow::height / 2);
		}
		else if (key == 2 && XPos < GLFWWindow::width / 2)
		{
			return XPos / (GLFWWindow::width / 2);
		}
		else if (key == 3 && XPos > GLFWWindow::width / 2)
		{
			return (XPos - (GLFWWindow::width / 2)) / (GLFWWindow::width / 2);
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

float GLFWInput::GamePadInput(int key)
{
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

	int keySlot = key - (key % 2) - 14;

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
