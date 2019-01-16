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
	EntityManager * entityManager;

public:
	explicit InputSystem();

	InputSystem& operator=(const InputSystem&) = delete;
	InputSystem(InputSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	static void Input(vector<InputFunction> * inputs, Entity * entity);
	~InputSystem();
};

