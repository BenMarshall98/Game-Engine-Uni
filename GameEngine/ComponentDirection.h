#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

using namespace glm;

class ComponentDirection : public iComponent
{
private:
	quat updateDirection;
	quat renderDirection;

public:
	ComponentDirection(vec3 & pDirection, float pAngle) : updateDirection(angleAxis(pAngle, pDirection)), renderDirection(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection();
	
	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline const quat & GetUpdateDirection() const
	{
		return updateDirection;
	}

	inline void SetUpdateDirection(const quat & pDirection)
	{
		updateDirection = pDirection;
	}

	inline const quat & GetRenderDirection() const
	{
		return renderDirection;
	}
};

