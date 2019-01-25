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
	explicit Entity(const string & entityName) : mEntityName(entityName), mEntityID(nextID)
	{
		nextID++;
	}

	~Entity()
	{
	}

	inline int GetID() const
	{
		return mEntityID;
	}

	inline const string & GetName() const
	{
		return mEntityName;
	}
};