#pragma once

#include "glm/glm.hpp"

class Vec3AnimKey
{
private:
	glm::vec3 value;
	float time;

public:
	Vec3AnimKey(const float pTime, glm::vec3 & pValue) : value(pValue), time(pTime) {}
	~Vec3AnimKey();

	//Gets the key frame time
	inline float GetTime()
	{
		return time;
	}

	//Gets the key frame value
	inline glm::vec3 & GetValue()
	{
		return value;
	}
};