#pragma once
#include "InputMapping.h"
#include "Entity.h"
#include <vector>

using namespace std;

class InputFunction
{
private:
	string inputFunction;
	vector<GameInput> Inputs;

public:
	explicit InputFunction(string pInputFunction) : inputFunction(pInputFunction) {}

	void AddInput(GameInput input);
	void RemoveInput(GameInput input);
	void SwapFunction(string pInputFunction);
	void Update(Entity * entity);

	~InputFunction();
};

