#pragma once

#include <string>

class Input
{
protected:
	static bool mouseAllowed;
	static bool keyboardAllowed;
	static bool gamepadAllowed;
	static std::string controller;

public:
	Input();
	virtual ~Input();

	virtual float KeyboardInput(int key) const = 0;
	virtual float MouseInput(int key) const = 0;
	virtual float GamePadInput(int key) const = 0;

	void AllowMouseInput(bool allowMouse);
	void AllowKeyboardInput(bool allowKeyboard);
	void AllowGamePadInput(bool allowGamePad);

	static void ControllerSetup(std::string pController);

	//Gets the controller setup
	static inline std::string GetController()
	{
		return controller;
	}
};

