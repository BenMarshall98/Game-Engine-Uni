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
	ComponentDirection(glm::vec3 & pDirection, float pAngle) :
		iComponent(ComponentType::COMPONENT_DIRECTION), updateDirection(glm::angleAxis(glm::radians(pAngle), pDirection)), renderDirection(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection();

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

