#include "Input.h"

bool Input::mouseAllowed = true;
bool Input::keyboardAllowed = true;
bool Input::gamepadAllowed = true;
std::string Input::controller = "";

Input::Input()
{
}

Input::~Input()
{
}

//Sets if mouse input is allowed
void Input::AllowMouseInput(const bool allowMouse)
{
	mouseAllowed = allowMouse;
}

//Sets if keyboard input is allowed
void Input::AllowKeyboardInput(const bool allowKeyboard)
{
	keyboardAllowed = allowKeyboard;
}

//Sets if gamepad input is allowed
void Input::AllowGamePadInput(const bool allowGamePad)
{
	gamepadAllowed = allowGamePad;
}

//SEt controller to use
void Input::ControllerSetup(std::string pController)
{
	if (pController == "KeyboardOnly")
	{
		mouseAllowed = false;
		keyboardAllowed = true;
		gamepadAllowed = false;
	}
	else if (pController == "KeyboardAndMouse")
	{
		mouseAllowed = true;
		keyboardAllowed = true;
		gamepadAllowed = false;
	}
	else if (pController == "GamepadOnly")
	{
		mouseAllowed = false;
		keyboardAllowed = false;
		gamepadAllowed = true;
	}

	controller = pController;
}
