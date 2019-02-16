#pragma once

#include "Entity.h"

class EntitySound
{
private:
	void * source;
	Entity * entity;

public:

	explicit EntitySound(void * const pSource, Entity * const pEntity) :
		source(pSource), entity(pEntity) {}

	~EntitySound();

	EntitySound(const EntitySound&) = delete;
	EntitySound& operator=(const EntitySound&) = delete;

	inline void * GetSource() const
	{
		return source;
	}

	inline Entity * GetEntity() const
	{
		return entity;
	}
};