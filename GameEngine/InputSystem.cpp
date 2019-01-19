#include "InputSystem.h"
#include "InputManager.h"
#include "ComponentInput.h"

#include <string>
#include <algorithm>

InputSystem::InputSystem()
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { ComponentType::COMPONENT_INPUT };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void InputSystem::RemoveEntity(Entity * pEntity)
{
	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);
	
	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void InputSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentInput = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_INPUT);

		vector<InputFunction> * inputs = dynamic_cast<ComponentInput *>(componentInput)->GetUpdateInputs();

		Input(inputs, EntityList[i]);
	}
}

void InputSystem::Input(vector<InputFunction>  * inputs, Entity * entity)
{	
	for (int i = 0; i < inputs->size(); i++)
	{
		inputs->at(i).Update(entity);
	}
}

InputSystem::~InputSystem()
{
}
