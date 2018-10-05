#pragma once

#include <string>

using namespace std;

class Entity
{
private:
	static int nextID;

	string mEntityName;
	int mEntityID;
	

public:
	Entity(string entityName) : mEntityName(entityName), mEntityID(nextID)
	{
		nextID++;
	}
	~Entity() {};

	inline int GetID()
	{
		return mEntityID;
	}

	inline string GetName()
	{
		return mEntityName;
	}
};