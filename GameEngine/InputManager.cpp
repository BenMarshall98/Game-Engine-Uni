#include "InputManager.h"

InputManager * InputManager::instance = nullptr;

InputManager::~InputManager()
{
}

float InputManager::GetInputValue(const vector<GameInput> & gameInputs) const
{
	float value = 0;

	for (int i = 0; i < gameInputs.size(); i++)
	{
		float newValue = 0;
		if ((gameInputs.at(i) & GAMEPAD << DEVICE) == GAMEPAD << DEVICE)
		{
			const int key = (gameInputs.at(i) - (GAMEPAD << DEVICE)) >> CODE;
			newValue = inputDevice->GamePadInput(key);
		}
		else if ((gameInputs.at(i) & KEYBOARD << DEVICE) == KEYBOARD << DEVICE)
		{
			const int key = (gameInputs.at(i) - (KEYBOARD << DEVICE)) >> CODE;
			newValue = inputDevice->KeyboardInput(key);
		}
		else if ((gameInputs.at(i) & MOUSE << DEVICE) == MOUSE << DEVICE)
		{
			const int key = (gameInputs.at(i) - (MOUSE << DEVICE)) >> CODE;
			newValue = inputDevice->MouseInput(key);
		}

		if (newValue > value)
		{
			value = newValue;
		}
	}

	return value;
}