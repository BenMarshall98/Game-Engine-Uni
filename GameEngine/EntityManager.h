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

	Entity* CreateEntity(const string & entityName = string(""));
	Entity* GetEntityByName(const string & entityName);
	
	inline void AddToDeleteList(Entity * entity)
	{
		ToDeleteList.push_back(entity);
	}

	void AddComponentToEntity(Entity * entity, iComponent * component);
	void RemoveComponentFromEntity(Entity * entity, iComponent * component);
	iComponent * GetComponentOfEntity(Entity * entity, ComponentType componentName);
	vector<Entity *> GetAllEntitiesWithComponents(const ComponentType * componentName, int size);

	void Update();
	void Swap();

	~EntityManager();
};

