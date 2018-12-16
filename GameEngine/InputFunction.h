#pragma once
#include "InputMapping.h"
#include "Entity.h"
#include <vector>

using namespace std;

class InputFunction
{
private:
	typedef void(*GameInputFunction)(float, Entity *);
	GameInputFunction inputFunction;
	vector<GameInput> Inputs;

public:
	explicit InputFunction(GameInputFunction pInputFunction) : inputFunction(pInputFunction) {}

	void AddInput(GameInput input);
	void RemoveInput(GameInput input);
	void SwapFunction(GameInputFunction pInputFunction);
	void Update(Entity * entity);

	~InputFunction();
};

