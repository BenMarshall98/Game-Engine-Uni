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
	std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it;
	for (it = ComponentList.begin(); it != ComponentList.end(); ++it)
	{
		std::map<Entity *, iComponent *> & entities = it->second;
		const std::map<Entity *, iComponent *>::iterator find = entities.find(entity);

		if (find == entities.end())
		{
			continue;
		}

		iComponent * const Component = find->second;
		delete Component;

		ComponentList[it->first].erase(entity);
	}

	const std::vector<Entity *>::iterator object = find(EntityList.begin(), EntityList.end(), entity);

	if (object != EntityList.end())
	{
		EntityList.erase(object);
	}

	delete entity;
	entity = nullptr;
}

void EntityManager::AddComponentToEntity(Entity * entity, iComponent * component)
{
	ComponentType componentType = component->GetComponentName();
	const std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentType);

	if (it != ComponentList.end())
	{
		ComponentList[componentType].insert(std::pair<Entity *, iComponent *>(entity, component));
	}
	else
	{
		std::map<Entity *, iComponent *> ComponentDetails;
		ComponentList.insert(std::pair<ComponentType, std::map<Entity *, iComponent *>>(componentType, ComponentDetails));
		ComponentList[componentType].insert(std::pair<Entity *, iComponent *>(entity, component));
	}
}

void EntityManager::RemoveComponentFromEntity(Entity * const entity, iComponent * const component)
{
	const ComponentType componentType = component->GetComponentName();
	const std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentType);

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

iComponent * EntityManager::GetComponentOfEntity(Entity * const entity, const ComponentType componentName)
{
	std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it = ComponentList.find(componentName);

	if (it != ComponentList.end())
	{
		std::map<Entity *, iComponent *> & entities = it->second;
		const std::map<Entity *, iComponent *>::iterator find = entities.find(entity);

		if (find != entities.end())
		{
			return find->second;
		}
	}

	return nullptr;
}

std::vector<Entity *> EntityManager::GetAllEntitiesWithComponents(std::vector<ComponentType>componentName)
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

bool EntityManager::CheckEntityHasComponents(Entity * const entity, std::vector<ComponentType> componentName)
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
	std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it;
	for (it = ComponentList.begin(); it != ComponentList.end(); ++it)
	{
		std::map<Entity *, iComponent *> & entities = it->second;
		std::map<Entity *, iComponent *>::iterator component;

		for (component = entities.begin(); component != entities.end(); ++component)
		{
			component->second->RenderSwap();
		}
	}
}

void EntityManager::Clear()
{
	std::map<ComponentType, std::map<Entity *, iComponent *>>::iterator it;
	for (it = ComponentList.begin(); it != ComponentList.end(); ++it)
	{
		std::map<Entity *, iComponent *> & entities = it->second;
		std::map<Entity *, iComponent *>::iterator component;

		for (component = entities.begin(); component != entities.end(); ++component)
		{
			delete component->second;
		}

		entities.clear();
	}

	ComponentList.clear();

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