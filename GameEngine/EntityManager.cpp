#include "EntityManager.h"

Entity * EntityManager::CreateEntity(string entityName)
{
	Entity * entity = new Entity(entityName);

	EntityList.push_back(entity);

	return entity;
}

Entity * EntityManager::GetEntityByName(string entityName)
{
	if (entityName == "")
	{
		return nullptr;
	}

	for (int i = 0; i < EntityList.size; i++)
	{
		if (EntityList[i]->GetName() == entityName)
		{
			return EntityList[i];
		}
	}

	return nullptr;
}

void EntityManager::RemoveEntity(Entity* entity)
{
	for (int i = 0; i < ComponentList.size; i++)
	{
		ComponentList[i].
	}
}
