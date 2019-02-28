#pragma once

#include "glm/glm.hpp"

using namespace glm;

class Vec3AnimKey
{
private:
	vec3 value;
	float time;

public:
	Vec3AnimKey(float pTime, vec3 pValue) : value(pValue), time(pTime) {}
	~Vec3AnimKey();

	inline float GetTime()
	{
		return time;
	}

	inline vec3 GetValue()
	{
		return value;
	}
};