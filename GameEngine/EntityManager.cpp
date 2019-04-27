#include "EntityManager.h"

//Creates a new entity
Entity * EntityManager::CreateEntity(const std::string & entityName)
{
	Entity * const entity = new Entity(entityName);

	EntityList.push_back(entity);

	return entity;
}

//Gets entity by its name 
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

//Sets the entity to be updated in the system list
void EntityManager::FinishEntity(Entity * const entity)
{
	ToUpdateList.push_back(entity);
}

//Remove entity
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

//Adds component to the game engine
void EntityManager::AddComponentToEntity(Entity * entity, iComponent * component)
{
	entity->AddComponent(component);
}

//Gets component from the entity
iComponent * EntityManager::GetComponentOfEntity(Entity * const entity, const ComponentType componentName)
{
	return entity->GetComponentOfEntity(componentName);
}

//Checks a list of all entities that contains the required components 
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

//Checks to see if entity has required components
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

//Makes sure that the systems are upto date, with new anad deleted entities
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

//Keeps entities concurrency complient
void EntityManager::Swap()
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		EntityList.at(i)->Swap();
	}
}

//Removes all components, ready for the next scene
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