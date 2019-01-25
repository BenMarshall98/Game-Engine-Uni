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

	explicit InputManager(GLFWInput * const pInputDevice) : inputDevice(pInputDevice) {}

public:

	InputManager & operator=(const InputManager&) = delete;
	InputManager(InputManager&) = delete;

	inline static InputManager * Instance(GLFWInput * pInputDevice = nullptr)
	{
		if (instance == nullptr)
		{
			instance = new InputManager(pInputDevice);
		}
		return instance;
	}

	float GetInputValue(vector<GameInput> & gameInputs);

	inline GLFWInput * GetInputReader() const
	{
		return inputDevice;
	}

	~InputManager();
};

