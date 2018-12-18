#pragma once

#include "iComponent.h"
#include "Entity.h"

#include "SystemManager.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

class EntityManager
{
private:
	static EntityManager * instance;
	map<ComponentType, map<Entity *, iComponent *>> ComponentList;
	vector<Entity *> EntityList;
	vector<Entity *> ToDeleteList;

	void RemoveEntity(Entity* entity);

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

	Entity* CreateEntity(string entityName = "");
	Entity* GetEntityByName(string entityName);
	
	void AddToDeleteList(Entity * entity);
	void AddComponentToEntity(Entity * entity, iComponent * component);
	void RemoveComponentFromEntity(Entity * entity, iComponent * component);
	iComponent * GetComponentOfEntity(Entity * entity, ComponentType componentName);
	vector<Entity *> GetAllEntitiesWithComponents(ComponentType * componentName, int size);

	void Update(SystemManager & systemManager);

	~EntityManager() {};
};

