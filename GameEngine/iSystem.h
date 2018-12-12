#pragma once
#include "Entity.h"
class iSystem
{
public:
	iSystem();
	~iSystem();

	virtual void Action() = 0;
	virtual void RemoveEntity(Entity * pEntity) = 0;
};

