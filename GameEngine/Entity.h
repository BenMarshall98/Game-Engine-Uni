#pragma once

#include <string>

class Entity
{
private:
	static int nextID;

	std::string mEntityName;
	int mEntityID;
	

public:
	explicit Entity(const std::string & entityName) : mEntityName(entityName), mEntityID(nextID)
	{
		nextID++;
	}

	Entity& operator= (const Entity&) = delete;
	Entity(Entity&) = delete;

	~Entity()
	{
	}

	inline int GetID() const
	{
		return mEntityID;
	}

	inline const std::string & GetName() const
	{
		return mEntityName;
	}
};