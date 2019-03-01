#pragma once

#include "glm/glm.hpp"

class Vec3AnimKey
{
private:
	glm::vec3 value;
	float time;

public:
	Vec3AnimKey(float pTime, glm::vec3 pValue) : value(pValue), time(pTime) {}
	~Vec3AnimKey();

	inline float GetTime()
	{
		return time;
	}

	inline glm::vec3 GetValue()
	{
		return value;
	}
};