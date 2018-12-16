#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPosition : public iComponent
{
private:
	vec3 position;
public:
	explicit ComponentPosition(vec3 pPosition) : position(pPosition) {}
	~ComponentPosition() {}

	ComponentType GetComponentName() override;
	void Swap() override {}

	inline vec3 GetPosition() const
	{
		return position;
	}

	inline void SetPosition(vec3 pPosition)
	{
		position = pPosition;
	}
};

