#pragma once
#include "GLFWInput.h"
#include "InputMapping.h"

#include <vector>

using namespace std;

class InputManager
{
private:
	static InputManager * instance;
	GLFWInput * inputDevice;

	InputManager(GLFWInput * pInputDevice) : inputDevice(pInputDevice) {}

public:

	inline static InputManager * Instance(GLFWInput * pInputDevice = nullptr)
	{
		if (instance == nullptr)
		{
			instance = new InputManager(pInputDevice);
		}
		return instance;
	}

	float GetInputValue(vector<GameInput> gameInputs);

	~InputManager();
};

