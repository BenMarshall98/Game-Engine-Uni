#pragma once

#include "iComponent.h"
#include "InputFunction.h"

#include <vector>

class ComponentInput : public iComponent
{
private:
	std::vector<InputFunction *> * inputs;

public:
	explicit ComponentInput(std::vector<InputFunction *> * const pInputs) :
		iComponent(ComponentType::COMPONENT_INPUT), inputs(pInputs) {}
	~ComponentInput();

	ComponentInput& operator=(const ComponentInput&) = delete;
	ComponentInput(ComponentInput&) = delete;

	void RenderSwap() override;

	//Gets the list of input functions
	inline std::vector<InputFunction *> * const GetUpdateInputs() const
	{
		return inputs;
	}
};

