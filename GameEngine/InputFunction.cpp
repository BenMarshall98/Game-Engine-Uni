#include "InputFunction.h"
#include "InputManager.h"
#include "ScriptingManager.h"
#include <algorithm>

void InputFunction::AddInput(const GameInput input)
{
	if (find(Inputs.begin(), Inputs.end(), input) == Inputs.end())
	{
		Inputs.push_back(input);
	}
	
}

void InputFunction::RemoveInput(const GameInput input)
{
	std::vector<GameInput>::iterator it;

	it = find(Inputs.begin(), Inputs.end(), input);

	if (it != Inputs.end())
	{
		Inputs.erase(it);
	}
}

void InputFunction::Update(Entity * const entity, float timePeriod) const
{
	InputManager * const inputManager = InputManager::Instance();
	const float value = inputManager->GetInputValue(Inputs);
	ScriptingManager::Instance()->RunScriptFromInput(inputFunction, entity, value, timePeriod);
}

InputFunction::~InputFunction()
{
}
