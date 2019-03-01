#pragma once

#include "glm/glm.hpp"

class LocationSound
{
private:
	glm::vec3 location;
	void * source;

public:
	explicit LocationSound(void * const pSource, const glm::vec3 & pLocation) :
		location(pLocation), source(pSource) {}
	~LocationSound();

	LocationSound(const LocationSound&) = delete;
	LocationSound& operator=(const LocationSound&) = delete;

	inline void * GetSource() const
	{
		return source;
	}
};