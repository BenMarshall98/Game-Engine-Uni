#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

class ComponentDirection : public iComponent
{
private:
	glm::quat updateDirection;
	glm::quat renderDirection;

public:
	ComponentDirection(glm::vec3 & pDirection, float pAngle) : updateDirection(glm::angleAxis(glm::radians(pAngle), pDirection)), renderDirection(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection();
	
	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline const glm::quat & GetUpdateDirection() const
	{
		return updateDirection;
	}

	inline void SetUpdateDirection(const glm::quat & pDirection)
	{
		updateDirection = pDirection;
	}

	inline const glm::quat & GetRenderDirection() const
	{
		return renderDirection;
	}
};

