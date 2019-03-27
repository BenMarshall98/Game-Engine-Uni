#include "Input.h"

bool Input::mouseAllowed = true;
bool Input::keyboardAllowed = true;
bool Input::gamepadAllowed = true;

Input::Input()
{
}

Input::~Input()
{
}

void Input::AllowMouseInput(const bool allowMouse)
{
	mouseAllowed = allowMouse;
}

void Input::AllowKeyboardInput(const bool allowKeyboard)
{
	keyboardAllowed = allowKeyboard;
}

void Input::AllowGamePadInput(const bool allowGamePad)
{
	gamepadAllowed = allowGamePad;
}

void Input::ControllerSetup(std::string controller)
{
	if (controller == "KeyboardOnly")
	{
		mouseAllowed = false;
		keyboardAllowed = true;
		gamepadAllowed = false;
	}
	else if (controller == "KeyboardAndMouse")
	{
		mouseAllowed = true;
		keyboardAllowed = true;
		gamepadAllowed = false;
	}
	else if (controller == "GamepadOnly")
	{
		mouseAllowed = false;
		keyboardAllowed = false;
		gamepadAllowed = true;
	}
}
