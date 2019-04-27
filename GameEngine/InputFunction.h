#pragma once
#include "InputMapping.h"
#include "Entity.h"
#include <vector>

class InputFunction
{
private:
	std::string inputFunction;
	std::vector<GameInput> Inputs;

public:
	explicit InputFunction(const std::string & pInputFunction) : inputFunction(pInputFunction) {}

	void AddInput(GameInput input);
	void RemoveInput(GameInput input);

	//Changes the Lua function to be called
	inline void SwapFunction(const std::string & pInputFunction)
	{
		inputFunction = pInputFunction;
	}
	void Update(Entity * entity, float timePeriod) const;

	~InputFunction();
};

