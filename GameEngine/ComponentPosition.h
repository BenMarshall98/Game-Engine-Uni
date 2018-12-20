#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPosition : public iComponent
{
private:
	vec3 currentPosition;
	vec3 newPosition;

public:
	explicit ComponentPosition(vec3 pPosition) : currentPosition(pPosition), newPosition(pPosition) {}
	~ComponentPosition() {}

	ComponentType GetComponentName() override;
	void Swap() override;

	inline vec3 GetPosition() const
	{
		return currentPosition;
	}

	inline void SetPosition(vec3 pPosition)
	{
		newPosition = pPosition;
	}
};

