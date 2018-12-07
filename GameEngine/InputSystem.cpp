#include "InputSystem.h"
#include "InputManager.h"
#include "ComponentInput.h"

#include <string>

InputSystem::InputSystem(EntityManager & pEntityManager) : entityManager(pEntityManager)
{
	ComponentType componentTypes[] = { COMPONENT_INPUT };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void InputSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentInput = entityManager.GetComponentOfEntity(EntityList[i], COMPONENT_INPUT);

		vector<InputFunction> inputs = ((ComponentInput *)componentInput)->GetInputs();

		Input(inputs, EntityList[i]);
	}
}

void InputSystem::Input(vector<InputFunction> inputs, Entity * entity)
{
	InputManager * inputManager = InputManager::Instance();
	
	for (int i = 0; i < inputs.size(); i++)
	{
		inputs.at(i).Update(entity);
	}
}

InputSystem::~InputSystem()
{
}
