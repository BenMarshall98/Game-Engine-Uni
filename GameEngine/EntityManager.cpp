#include "EntityManager.h"

Entity * EntityManager::CreateEntity(const std::string & entityName)
{
	Entity * const entity = new Entity(entityName);

	EntityList.push_back(entity);

	return entity;
}

Entity * EntityManager::GetEntityByName(const std::string & entityName)
{
	if (entityName.compare("") == 0)
	{
		return nullptr;
	}

	for (int i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i]->GetName().compare(entityName) == 0)
		{
			return EntityList[i];
		}
	}

	return nullptr;
}

void EntityManager::FinishEntity(Entity * const entity)
{
	ToUpdateList.push_back(entity);
}

void EntityManager::RemoveEntity(Entity * entity)
{
	const std::vector<Entity *>::iterator object = find(EntityList.begin(), EntityList.end(), entity);

	if (object != EntityList.end())
	{
		EntityList.erase(object);
	}

	entity->RemoveAllComponents();
	delete entity;
	entity = nullptr;
}

void EntityManager::AddComponentToEntity(Entity * entity, iComponent * component)
{
	entity->AddComponent(component);
}

void EntityManager::RemoveComponentFromEntity(Entity * const entity, iComponent * const component)
{
	entity->RemoveComponent(component);
}

iComponent * EntityManager::GetComponentOfEntity(Entity * const entity, const ComponentType componentName)
{
	return entity->GetComponentOfEntity(componentName);
}

std::vector<Entity *> EntityManager::GetAllEntitiesWithComponents(const std::vector<ComponentType> & componentName)
{
	std::vector<Entity *> entities;

	for (int i = 0; i < EntityList.size(); i++)
	{
		Entity * const entity = EntityList.at(i);
		bool containsComponents = true;

		for (int j = 0; j < componentName.size(); j++)
		{
			if (GetComponentOfEntity(entity, componentName[j]) == nullptr)
			{
				containsComponents = false;
				break;
			}
		}

		if (containsComponents)
		{
			entities.push_back(entity);
		}
	}

	return entities;
}

bool EntityManager::CheckEntityHasComponents(Entity * const entity, const std::vector<ComponentType> & componentName)
{
	bool containsComponents = true;

	for (int i = 0; i < componentName.size(); i++)
	{
		if (GetComponentOfEntity(entity, componentName[i]) == nullptr)
		{
			containsComponents = false;
			break;
		}
	}

	return containsComponents;
}

void EntityManager::Update()
{
	SystemManager * const systemManager = SystemManager::Instance();

	for (int i = 0; i < ToUpdateList.size(); i++)
	{
		systemManager->AddEntityToSystems(ToUpdateList[i]);
	}

	for (int i = 0; i < ToDeleteList.size(); i++)
	{
		systemManager->RemoveEntityFromSystems(ToDeleteList[i]);
		RemoveEntity(ToDeleteList[i]);
	}

	ToUpdateList.clear();
	ToDeleteList.clear();
}

void EntityManager::Swap()
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		EntityList.at(i)->Swap();
	}
}

void EntityManager::Clear()
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		EntityList.at(i)->RemoveAllComponents();
	}

	for (int i = 0; i < EntityList.size(); i++)
	{
		delete EntityList.at(i);
		EntityList.at(i) = nullptr;
	}

	EntityList.clear();
}

EntityManager::~EntityManager()
{
	Clear();
	instance = nullptr;
}

EntityManager * EntityManager::instance = nullptr;