#include "InputFunction.h"
#include <algorithm>

void InputFunction::AddInput(GameInput input)
{
	if (find(Inputs.begin(), Inputs.end(), input) == Inputs.end())
	{
		Inputs.push_back(input);
	}
	
}

void InputFunction::RemoveInput(GameInput input)
{
	vector<GameInput>::iterator it;

	it = find(Inputs.begin(), Inputs.end(), input);

	if (it != Inputs.end())
	{
		Inputs.erase(it);
	}
}

void InputFunction::SwapFunction(GameInputFunction pInputFunction)
{
	inputFunction = pInputFunction;
}

void InputFunction::Update()
{
	//TODO: Implement
}

InputFunction::~InputFunction()
{
}
