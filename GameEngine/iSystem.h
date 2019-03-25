#pragma once
#include "Entity.h"
#include "iComponent.h"
#include <vector>

class iSystem
{
protected:
	const std::vector<ComponentType> componentTypes;
	std::vector<Entity *> EntityList;

public:
	explicit iSystem(const std::vector<ComponentType> & pComponentTypes);
	virtual ~iSystem();

	iSystem& operator=(const iSystem&) = delete;
	iSystem(iSystem&) = delete;

	virtual void Action() = 0;

	virtual void RemoveEntity(Entity * pEntity);
	virtual void AddEntity(Entity * pEntity);
};

