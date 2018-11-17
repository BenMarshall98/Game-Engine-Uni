#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "InputFunction.h"

#include <vector>

using namespace std;

class InputSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	EntityManager & entityManager;

public:
	InputSystem(EntityManager & entityManager);
	void Action(void);
	void Input(vector<InputFunction> inputs, Entity * entity);
	~InputSystem();
};

