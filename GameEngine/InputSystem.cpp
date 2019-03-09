#include "InputSystem.h"
#include "InputManager.h"
#include "ComponentInput.h"

#include <string>
#include <algorithm>

InputSystem::InputSystem() : entityManager(EntityManager::Instance())
{
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void InputSystem::RemoveEntity(Entity * pEntity)
{
	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);
	
	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void InputSystem::AddEntity(Entity * pEntity)
{
	bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		containsEntity = true;
	}

	if (containsEntity && !containsComponents)
	{
		EntityList.erase(it);
	}
	else if (!containsEntity && containsComponents)
	{
		EntityList.push_back(pEntity);
	}
}

void InputSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentInput = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_INPUT);

		std::vector<InputFunction *> * inputs = dynamic_cast<ComponentInput *>(componentInput)->GetUpdateInputs();

		Input(inputs, EntityList[i]);
	}
}

void InputSystem::Input(std::vector<InputFunction *> * inputs, Entity * entity)
{	
	for (int i = 0; i < inputs->size(); i++)
	{
		inputs->at(i)->Update(entity);
	}
}

InputSystem::~InputSystem()
{
}
