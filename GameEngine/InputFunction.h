#pragma once
#include "InputMapping.h"
#include <vector>

using namespace std;

class InputFunction
{
private:
	typedef void(*GameInputFunction)(float);
	GameInputFunction inputFunction;

	vector<GameInput> Inputs;

public:
	InputFunction(GameInputFunction pInputFunction) : inputFunction(pInputFunction) {}

	void AddInput(GameInput input);
	void RemoveInput(GameInput input);
	void SwapFunction(GameInputFunction pInputFunction);
	void Update();

	~InputFunction();
};

