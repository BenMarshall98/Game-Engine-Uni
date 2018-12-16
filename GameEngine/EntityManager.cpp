#include "EntityManager.h"
Entity * EntityManager::CreateEntity(string * entityName)
{
	Entity * entity = new Entity(entityName);

	EntityList.push_back(entity);

	return entity;
}

Entity * EntityManager::GetEntityByName(string & entityName)
{
	if (entityName == "")
	{
		return nullptr;
	}

	for (int i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i]->GetName()->compare(entityName))
		{
			return EntityList[i];
		}
	}

	return nullptr;
}

void EntityManager::RemoveEntity(Entity* entity)
{
	map<ComponentType, map<Entity *, iComponent *>>::iterator it;
	for (it = ComponentList.begin(); it != ComponentList.end(); ++it)
	{
		map<Entity *, iComponent *> & entities = it->second;
		map<Entity *, iComponent *>::iterator find = entities.find(entity);

		if (find == entities.end())
		{
			continue;
		}

		iComponent * Component = find->second;
		delete Component;

		ComponentList[it->first].erase(entity);
	}

	vector<Entity *>::iterator object = find(EntityList.begin(), EntityList.end(), entity);

	if (object != EntityList.end())
	{
		EntityList.erase(object);
	}

	delete entity;
}

void EntityManager::AddComponentToEntity(Entity * entity, iComponent * component)
{
	ComponentType componentType = component->GetComponentName();
	map<ComponentType, map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentType);

	if (it != ComponentList.end())
	{
		ComponentList[componentType].insert(pair<Entity *, iComponent *>(entity, component));
	}
	else
	{
		map<Entity *, iComponent *> ComponentDetails;
		ComponentList.insert(pair<ComponentType, map<Entity *, iComponent *>>(componentType, ComponentDetails));
		ComponentList[componentType].insert(pair<Entity *, iComponent *>(entity, component));
	}
}

void EntityManager::RemoveComponentFromEntity(Entity * entity, iComponent * component)
{
	ComponentType componentType = component->GetComponentName();
	map<ComponentType, map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentType);

	if (it != ComponentList.end())
	{
		ComponentList[componentType].erase(entity);

		if (ComponentList[componentType].size() == 0)
		{
			ComponentList.erase(componentType);
		}

		delete component;
	}
}

iComponent * EntityManager::GetComponentOfEntity(Entity * entity, ComponentType componentName)
{
	map<ComponentType, map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentName);

	if (it != ComponentList.end())
	{
		map<Entity *, iComponent *> & entities = it->second;
		map<Entity *, iComponent *>::iterator find = entities.find(entity);

		if (find != entities.end())
		{
			return find->second;
		}
	}

	return nullptr;
}

vector<Entity *> EntityManager::GetAllEntitiesWithComponents(ComponentType * componentName, int size)
{
	vector<Entity *> entities;

	for (int i = 0; i < EntityList.size(); i++)
	{
		Entity * entity = EntityList.at(i);
		bool containsComponents = true;

		for (int j = 0; j < size; j++)
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

void EntityManager::AddToDeleteList(Entity * entity)
{
	ToDeleteList.push_back(entity);
}

void EntityManager::Update(SystemManager & systemManager)
{
	for (int i = 0; i < ToDeleteList.size(); i++)
	{
		systemManager.RemoveEntityFromSystems(ToDeleteList[i]);
		RemoveEntity(ToDeleteList[i]);
	}

	ToDeleteList.clear();
}