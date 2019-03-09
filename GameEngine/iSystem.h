#pragma once
#include "Entity.h"
class iSystem
{
public:
	iSystem();
	virtual ~iSystem();

	virtual void Action() = 0;
	virtual void RemoveEntity(Entity * pEntity) = 0;
	virtual void AddEntity(Entity * pEntity) = 0;
};

