#pragma once

#include "glm/glm.hpp"

using namespace glm;

class Vec3AnimKey
{
private:
	float time;
	vec3 value;

public:
	Vec3AnimKey(float pTime, vec3 pValue) : time(pTime), value(pValue) {}
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