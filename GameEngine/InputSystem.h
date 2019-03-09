#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "InputFunction.h"

#include <vector>

class InputSystem : public iSystem
{
private:
	std::vector<Entity *> EntityList;
	EntityManager * entityManager;

	const ComponentType componentTypes[1] = {
		ComponentType::COMPONENT_INPUT
	};

	static void Input(std::vector<InputFunction *> * inputs, Entity * entity);

public:
	InputSystem();

	InputSystem& operator=(const InputSystem&) = delete;
	InputSystem(InputSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	void AddEntity(Entity * pEntity) override;
	
	~InputSystem();
};

