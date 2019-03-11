#pragma once
class Input
{
protected:
	bool mouseAllowed = true;
	bool keyboardAllowed = true;
	bool gamepadAllowed = true;

public:
	Input();
	virtual ~Input();

	virtual float KeyboardInput(int key) const = 0;
	virtual float MouseInput(int key) const = 0;
	virtual float GamePadInput(int key) const = 0;

	void AllowMouseInput(bool allowMouse);
	void AllowKeyboardInput(bool allowKeyboard);
	void AllowGamePadInput(bool allowGamePad);
};

