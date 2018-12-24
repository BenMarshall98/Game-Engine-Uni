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

	void RenderSwap() override {};

	inline vector<InputFunction> * GetUpdateInputs()
	{
		return inputs;
	}
};

