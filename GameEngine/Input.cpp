#include "Input.h"



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
