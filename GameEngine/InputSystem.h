#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "InputFunction.h"

#include <vector>

class InputSystem : public iSystem
{
private:
	static void Input(std::vector<InputFunction *> * inputs, Entity * entity);

public:
	InputSystem();

	InputSystem& operator=(const InputSystem&) = delete;
	InputSystem(InputSystem&) = delete;

	void Action(void) override;
	
	~InputSystem();
};

