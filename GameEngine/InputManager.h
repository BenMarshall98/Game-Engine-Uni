#pragma once
#include "GLFWInput.h"
#include "InputMapping.h"

#include <vector>

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

	float GetInputValue(const std::vector<GameInput> & gameInputs) const;

	//Gets the input class used
	inline Input * GetInputReader() const
	{
		return inputDevice;
	}

	~InputManager();
};

