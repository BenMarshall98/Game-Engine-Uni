#pragma once

#include "Entity.h"
#include "Source.h"

class EntitySound
{
private:
	Source * source;
	Entity * entity;

public:

	explicit EntitySound(Source * const pSource, Entity * const pEntity) :
		source(pSource), entity(pEntity) {}

	~EntitySound();

	EntitySound(const EntitySound&) = delete;
	EntitySound& operator=(const EntitySound&) = delete;

	inline Source * GetSource() const
	{
		return source;
	}

	inline Entity * GetEntity() const
	{
		return entity;
	}
};