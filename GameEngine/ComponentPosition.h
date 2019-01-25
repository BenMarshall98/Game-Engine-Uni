#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPosition : public iComponent
{
private:
	vec3 updatePosition;
	vec3 renderPosition;

public:
	explicit ComponentPosition(vec3 & pPosition) : updatePosition(pPosition), renderPosition(pPosition) {}
	~ComponentPosition();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline const vec3 & GetUpdatePosition() const
	{
		return updatePosition;
	}

	inline void SetUpdatePosition(vec3 & pPosition)
	{
		updatePosition = pPosition;
	}

	inline const vec3 & GetRenderPosition()
	{
		return renderPosition;
	}
};

