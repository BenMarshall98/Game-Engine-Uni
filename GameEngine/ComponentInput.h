#pragma once

#include "iComponent.h"
#include "InputFunction.h"

#include <vector>

using namespace std;

class ComponentInput : public iComponent
{
private:
	vector<InputFunction> * inputs;

public:
	explicit ComponentInput(vector<InputFunction> * pInputs) : inputs(pInputs) {}
	~ComponentInput() {}

	ComponentType GetComponentName() override;
	void Swap() override {}

	inline vector<InputFunction> * GetInputs()
	{
		return inputs;
	}
};

