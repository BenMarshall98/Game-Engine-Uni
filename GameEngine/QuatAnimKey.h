#pragma once

#include "glm/gtc/quaternion.hpp"

using namespace glm;

class QuatAnimKey
{
private:
	quat value;
	float time;

public:
	QuatAnimKey(float pTime, quat pValue) : time(pTime), value(pValue) {}
	~QuatAnimKey();

	inline float GetTime()
	{
		return time;
	}

	inline quat GetValue()
	{
		return value;
	}
};