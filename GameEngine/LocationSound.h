#pragma once

#include "glm/glm.hpp"
#include "Source.h"

class LocationSound
{
private:
	glm::vec3 location;
	Source * source;

public:
	explicit LocationSound(Source * const pSource, const glm::vec3 & pLocation) :
		location(pLocation), source(pSource) {}
	~LocationSound();

	LocationSound(const LocationSound&) = delete;
	LocationSound& operator=(const LocationSound&) = delete;

	//Gets the used source
	inline Source * GetSource() const
	{
		return source;
	}
};