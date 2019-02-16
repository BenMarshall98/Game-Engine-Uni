#pragma once

#include "glm/glm.hpp"

using namespace glm;

class LocationSound
{
private:
	vec3 location;
	void * source;

public:
	explicit LocationSound(void * const pSource, const vec3 & pLocation) :
		location(pLocation), source(pSource) {}
	~LocationSound();

	LocationSound(const LocationSound&) = delete;
	LocationSound& operator=(const LocationSound&) = delete;

	inline void * GetSource() const
	{
		return source;
	}
};