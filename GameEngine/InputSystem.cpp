#include "InputSystem.h"
#include "InputManager.h"
#include "ComponentInput.h"

#include <string>
#include <algorithm>

InputSystem::InputSystem() : entityManager(EntityManager::Instance())
{
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void InputSystem::RemoveEntity(Entity * const pEntity)
{
	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);
	
	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void InputSystem::AddEntity(Entity * const pEntity)
{
	const bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

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
		iComponent * const componentInput = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_INPUT);

		std::vector<InputFunction *> * const inputs = dynamic_cast<ComponentInput *>(componentInput)->GetUpdateInputs();

		Input(inputs, EntityList[i]);
	}
}

void InputSystem::Input(std::vector<InputFunction *> * const inputs, Entity * const entity)
{	
	for (int i = 0; i < inputs->size(); i++)
	{
		inputs->at(i)->Update(entity);
	}
}

InputSystem::~InputSystem()
{
}
