#pragma once

#include "Entity.h"

class EntitySound
{
private:
	void * source;
	Entity * entity;

public:

	explicit EntitySound(void * pSource, Entity * pEntity) :
		source(pSource), entity(pEntity) {}

	~EntitySound();

	EntitySound(const EntitySound&) = delete;
	EntitySound& operator=(const EntitySound&) = delete;

	inline void * GetSource()
	{
		return source;
	}

	inline Entity * GetEntity()
	{
		return entity;
	}
};