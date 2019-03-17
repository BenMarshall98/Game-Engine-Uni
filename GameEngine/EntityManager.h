#pragma once

#include "iComponent.h"
#include "Entity.h"

#include "SystemManager.h"
#include <map>
#include <vector>
#include <string>

class EntityManager
{
private:
	static EntityManager * instance;
	std::map<ComponentType, std::map<Entity *, iComponent *>> ComponentList;
	std::vector<Entity *> EntityList;
	std::vector<Entity *> ToDeleteList;

	void RemoveEntity(Entity * entity);

	EntityManager() {};
public:
	
	inline static EntityManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new EntityManager();
		}
		return instance;
	}

	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	Entity* CreateEntity(const std::string & entityName = std::string(""));
	Entity* GetEntityByName(const std::string & entityName);

	void FinishEntity(Entity * entity);
	
	inline void AddToDeleteList(Entity * const entity)
	{
		const std::vector<Entity *>::iterator it = find(ToDeleteList.begin(), ToDeleteList.end(), entity);

		if (it == ToDeleteList.end())
		{
			ToDeleteList.push_back(entity);
		}	
	}

	void AddComponentToEntity(Entity * entity, iComponent * component);
	void RemoveComponentFromEntity(Entity * entity, iComponent * component);
	iComponent * GetComponentOfEntity(Entity * entity, ComponentType componentName);
	std::vector<Entity *> GetAllEntitiesWithComponents(std::vector<ComponentType> componentName);
	bool CheckEntityHasComponents(Entity * entity, std::vector<ComponentType> componentName);

	void Update();
	void Swap();
	void Clear();

	~EntityManager();
};

