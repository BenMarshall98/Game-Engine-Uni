#pragma once

#include "iComponent.h"
#include "InputFunction.h"

#include <vector>

class ComponentInput : public iComponent
{
private:
	std::vector<InputFunction *> * inputs;

public:
	explicit ComponentInput(std::vector<InputFunction *> * const pInputs) : inputs(pInputs) {}
	~ComponentInput();

	ComponentInput& operator=(const ComponentInput&) = delete;
	ComponentInput(ComponentInput&) = delete;

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline std::vector<InputFunction *> * const GetUpdateInputs() const
	{
		return inputs;
	}
};

