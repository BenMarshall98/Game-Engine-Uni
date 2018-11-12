#pragma once

#include "iComponent.h"
#include "InputFunction.h"

#include <vector>

using namespace std;

class ComponentInput : public iComponent
{
private:
	vector<InputFunction> inputs;

public:
	ComponentInput(vector<InputFunction> pInputs) : inputs(pInputs) {}
	~ComponentInput() {}

	string GetComponentName();
	void Swap() {}

	inline vector<InputFunction> GetInputs()
	{
		return inputs;
	}
};

