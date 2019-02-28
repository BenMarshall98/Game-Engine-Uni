#pragma once
#include "GLFWInput.h"
#include "InputMapping.h"

#include <vector>

using namespace std;

class InputManager
{
private:
	static InputManager * instance;
	Input * inputDevice;

	explicit InputManager(Input * const pInputDevice) : inputDevice(pInputDevice) {}

public:

	InputManager & operator=(const InputManager&) = delete;
	InputManager(InputManager&) = delete;

	inline static InputManager * Instance(Input * pInputDevice = nullptr)
	{
		if (instance == nullptr)
		{
			instance = new InputManager(pInputDevice);
		}
		return instance;
	}

	float GetInputValue(const vector<GameInput> & gameInputs) const;

	inline Input * GetInputReader() const
	{
		return inputDevice;
	}

	~InputManager();
};

