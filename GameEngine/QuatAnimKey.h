#pragma once

#include "glm/gtc/quaternion.hpp"

class QuatAnimKey
{
private:
	glm::quat value;
	float time;

public:
	QuatAnimKey(const float pTime, glm::quat & pValue) : time(pTime), value(pValue) {}
	~QuatAnimKey();

	//Gets the key frame time
	inline float GetTime()
	{
		return time;
	}

	//Gets the key fram value
	inline glm::quat & GetValue()
	{
		return value;
	}
};