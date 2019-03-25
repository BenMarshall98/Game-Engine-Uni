#include "iSystem.h"
#include "EntityManager.h"

iSystem::iSystem(const std::vector<ComponentType> & pComponentTypes) : componentTypes(pComponentTypes)
{
	EntityList = EntityManager::Instance()->GetAllEntitiesWithComponents(componentTypes);
}

iSystem::~iSystem()
{
}


void iSystem::AddEntity(Entity * const pEntity)
{
	EntityManager * const entityManager = EntityManager::Instance();
	const bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes);
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

void iSystem::RemoveEntity(Entity * const pEntity)
{
	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}