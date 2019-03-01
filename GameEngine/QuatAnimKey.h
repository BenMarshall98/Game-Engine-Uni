#pragma once

#include "glm/gtc/quaternion.hpp"

class QuatAnimKey
{
private:
	glm::quat value;
	float time;

public:
	QuatAnimKey(float pTime, glm::quat pValue) : time(pTime), value(pValue) {}
	~QuatAnimKey();

	inline float GetTime()
	{
		return time;
	}

	inline glm::quat GetValue()
	{
		return value;
	}
};