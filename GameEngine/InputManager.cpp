#include "InputManager.h"

InputManager::~InputManager()
{
}

float InputManager::GetInputValue(vector<GameInput> gameInputs)
{
	float value = 0;

	for (int i = 0; i < gameInputs.size(); i++)
	{
		float newValue = 0;
		if ((gameInputs.at(i) & KEYBOARD << DEVICE) == KEYBOARD << DEVICE)
		{
			int key = (gameInputs.at(i) - KEYBOARD << DEVICE) >> CODE;
			newValue = inputDevice->KeyboardInput(key);
		}
		else if ((gameInputs.at(i) & MOUSE << DEVICE) == MOUSE << DEVICE)
		{
			int key = (gameInputs.at(i) - MOUSE << DEVICE) >> CODE;
			newValue = inputDevice->MouseInput(key);
		}
		else
		{
			int key = (gameInputs.at(i) - GAMEPAD << DEVICE) >> CODE;
			newValue = inputDevice->GamePadInput(key);
		}

		if (newValue > value)
		{
			value = newValue;
		}
	}

	return value;
}