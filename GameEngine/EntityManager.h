#pragma once

#include "iComponent.h"
#include "Entity.h"

#include <map>
#include <vector>
#include <string>

using namespace std;

class EntityManager
{
private:
	map<string, map<Entity *, iComponent *>> ComponentList;
	vector<Entity *> EntityList;
public:
	EntityManager() {};

	Entity* CreateEntity(string entityName = "");
	Entity* GetEntityByName(string entityName);
	void RemoveEntity(Entity* entity);

	void AddComponentToEntity(Entity * entity, iComponent * component);
	void RemoveComponentFromEntity(Entity * entity, iComponent * component);
	iComponent * GetComponentOfEntity(Entity * entity, string componentName);
	vector<Entity *> GetAllEntitiesWithComponents(string * componentName, int size);

	~EntityManager() {};
};

