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
	explicit ComponentInput(vector<InputFunction> * const pInputs) : inputs(pInputs) {}
	~ComponentInput();

	ComponentInput& operator=(const ComponentInput&) = delete;
	ComponentInput(ComponentInput&) = delete;

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline vector<InputFunction> * const GetUpdateInputs() const
	{
		return inputs;
	}
};

