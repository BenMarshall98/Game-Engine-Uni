#pragma once

#include "iComponent.h"
#include <vector>
#include <string>

class Entity
{
private:
	static int nextID;

	std::string mEntityName;
	int mEntityID;

	std::vector<iComponent *> components = std::vector<iComponent *>();

public:
	explicit Entity(const std::string & entityName) : mEntityName(entityName), mEntityID(nextID)
	{
		nextID++;
	}

	Entity& operator= (const Entity&) = delete;
	Entity(Entity&) = delete;

	~Entity();

	inline int GetID() const
	{
		return mEntityID;
	}

	inline const std::string & GetName() const
	{
		return mEntityName;
	}

	void AddComponent(iComponent * component);
	void RemoveComponent(iComponent * component);
	void Swap();
	void RemoveAllComponents();

	iComponent * GetComponentOfEntity(ComponentType componentType);
};